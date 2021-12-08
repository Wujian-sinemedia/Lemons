namespace lemons::serializing
{

String memoryBlockToString (const MemoryBlock& block)
{
	return block.toBase64Encoding();
}

MemoryBlock memoryBlockFromString (const String& string)
{
	MemoryBlock block;
	block.fromBase64Encoding (string);
	return block;
}

/*---------------------------------------------------------------------------------------------------------------------------------*/

Image imageFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };
	juce::PNGImageFormat    format;

	return format.decodeImage (stream);
}

MemoryBlock imageToBinary (const Image& image)
{
	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };
	juce::PNGImageFormat     format;

	format.writeImageToStream (image, stream);

	return block;
}

/*-------------------------------------------------------------------------------------------------------------------------------------*/

MidiBuffer midiBufferFromFile (const juce::MidiFile& file, int trackToRead)
{
	MidiBuffer buffer;

	const auto addEventsFromTrack = [&] (int trackNum)
	{
		if (const auto* track = file.getTrack (trackNum))
		{
			for (const auto* holder : *track)
				buffer.addEvent (holder->message,
				                 juce::roundToInt (holder->message.getTimeStamp()));
		}
	};

	if (trackToRead > -1)
	{
		addEventsFromTrack (trackToRead);
	}
	else
	{
		for (int i = 0; i < file.getNumTracks(); ++i)
			addEventsFromTrack (i);
	}

	jassert (juce::roundToInt (file.getLastTimestamp()) == buffer.getLastEventTime());

	return buffer;
}

juce::MidiFile midiBufferToFile (const MidiBuffer& midi)
{
	juce::MidiFile file;

	const auto sequence = [&]() -> juce::MidiMessageSequence
	{
		juce::MidiMessageSequence seq;

		for (const auto& meta : midi)
			seq.addEvent (meta.getMessage());

		return seq;
	}();

	jassert (sequence.getNumEvents() == midi.getNumEvents());

	file.addTrack (sequence);

	jassert (file.getLastTimestamp() == midi.getLastEventTime());

	return file;
}

MidiBuffer midiFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };

	juce::MidiFile file;
	file.readFrom (stream);

	return midiBufferFromFile (file);
}

MemoryBlock midiToBinary (const MidiBuffer& midi)
{
	const auto file = midiBufferToFile (midi);

	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };

	file.writeTo (stream);

	return block;
}

}  // namespace lemons::serializing


/*-------------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

DataConversionTests::DataConversionTests()
    : CoreTest ("Data conversion functions")
{
}

void DataConversionTests::runTest()
{
	/*
	 - ValueTree to JSON
	 - ValueTree from JSON
	 */

	//    beginTest ("Audio buffers");
	//    runTypedTests<float>();
	//    runTypedTests<double>();

	beginTest ("MIDI buffers");

	MidiBuffer origMidi;

	constexpr auto numMidiEvents = 256;

	fillMidiBufferWithRandomEvents (origMidi, numMidiEvents, getRandom());

	{
		const auto subtest = beginSubtest ("MIDI buffer to/from MIDI file");

		const auto file    = serializing::midiBufferToFile (origMidi);
		const auto decoded = serializing::midiBufferFromFile (file);

		expect (midiBuffersAreEqual (origMidi, decoded));
	}

	const auto block   = serializing::midiToBinary (origMidi);
	const auto decoded = serializing::midiFromBinary (block);

	// expect (midiBuffersAreEqual (origMidi, decoded));

	beginTest ("Images");

	constexpr auto imageWidth  = 250;
	constexpr auto imageHeight = 250;

	juce::Image image { juce::Image::PixelFormat::RGB, imageWidth, imageHeight, true };

	const auto blob         = serializing::imageToBinary (image);
	const auto decodedImage = serializing::imageFromBinary (blob);

	expectEquals (decodedImage.getWidth(), imageWidth);
	expectEquals (decodedImage.getHeight(), imageHeight);


	beginTest ("Memory block to/from string");

	const auto memStr     = serializing::memoryBlockToString (block);
	const auto memDecoded = serializing::memoryBlockFromString (memStr);

	expect (block.matches (memDecoded.getData(), memDecoded.getSize()));
}

template <typename SampleType>
void DataConversionTests::runTypedTests()
{
	const auto subtest = beginSubtest (getPrecisionString<SampleType>() + " precision tests");

	constexpr auto numChannels = 2;
	constexpr auto numSamples  = 512;

	AudioBuffer<SampleType> origAudio { numChannels, numSamples };

	fillAudioBufferWithRandomNoise (origAudio, getRandom());

	const auto block   = serializing::audioToBinary (origAudio);
	const auto decoded = serializing::audioFromBinary<SampleType> (block);

	expectEquals (decoded.getNumChannels(), numChannels);
	expectEquals (decoded.getNumSamples(), numSamples);

	expect (buffersAreReasonablyEqual (origAudio, decoded));
}

template void DataConversionTests::runTypedTests<float>();
template void DataConversionTests::runTypedTests<double>();

}  // namespace lemons::tests

#endif
