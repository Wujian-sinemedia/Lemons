namespace lemons::binary
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

MidiBuffer midiBufferFromMidiFile (const juce::MidiFile& file)
{
	if (const auto* track = file.getTrack (0))
	{
		return [sequence = *track]() -> MidiBuffer
		{
			MidiBuffer buffer;

			for (const auto* holder : sequence)
				buffer.addEvent (holder->message,
				                 juce::roundToInt (holder->message.getTimeStamp()));

			jassert (buffer.getNumEvents() == sequence.getNumEvents());

			return buffer;
		}();
	}

	jassertfalse;
	return {};
}

juce::MidiFile midiBufferToMidiFile (const MidiBuffer& midi)
{
	juce::MidiFile file;

	const auto sequence = [&]() -> juce::MidiMessageSequence
	{
		juce::MidiMessageSequence seq;

		for (const auto meta : midi)
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

	return midiBufferFromMidiFile (file);
}

MemoryBlock midiToBinary (const MidiBuffer& midi)
{
	const auto file = midiBufferToMidiFile (midi);

	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };

	file.writeTo (stream);

	return block;
}

}  // namespace lemons::binary


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

		const auto file    = binary::midiBufferToMidiFile (origMidi);
		const auto decoded = binary::midiBufferFromMidiFile (file);

		expect (midiBuffersAreEqual (origMidi, decoded));
	}

	const auto block   = binary::midiToBinary (origMidi);
	const auto decoded = binary::midiFromBinary (block);

	// expect (midiBuffersAreEqual (origMidi, decoded));

	beginTest ("Images");

	constexpr auto imageWidth  = 250;
	constexpr auto imageHeight = 250;

	juce::Image image { juce::Image::PixelFormat::RGB, imageWidth, imageHeight, true };

	const auto blob         = binary::imageToBinary (image);
	const auto decodedImage = binary::imageFromBinary (blob);

	expectEquals (decodedImage.getWidth(), imageWidth);
	expectEquals (decodedImage.getHeight(), imageHeight);


	beginTest ("Memory block to/from string");

	const auto memStr     = binary::memoryBlockToString (block);
	const auto memDecoded = binary::memoryBlockFromString (memStr);

	expect (block.matches (memDecoded.getData(), memDecoded.getSize()));
}

template <typename SampleType>
void DataConversionTests::runTypedTests()
{
	const auto precisionString = []() -> String
	{
		if constexpr (std::is_same_v<SampleType, float>)
			return "Float";
		else
			return "Double";
	}();

	const auto subtest = beginSubtest (precisionString + " precision tests");

	constexpr auto numChannels = 2;
	constexpr auto numSamples  = 512;

	AudioBuffer<SampleType> origAudio { numChannels, numSamples };

	fillAudioBufferWithRandomNoise (origAudio, getRandom());

	const auto block   = binary::audioToBinary (origAudio);
	const auto decoded = binary::audioFromBinary<SampleType> (block);

	expectEquals (decoded.getNumChannels(), numChannels);
	expectEquals (decoded.getNumSamples(), numSamples);

	expect (buffersAreReasonablyEqual (origAudio, decoded));
}

template void DataConversionTests::runTypedTests<float>();
template void DataConversionTests::runTypedTests<double>();

}  // namespace lemons::tests

#endif
