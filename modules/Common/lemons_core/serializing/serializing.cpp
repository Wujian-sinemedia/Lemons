/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */


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

	fillMidiBufferWithRandomEvents (origMidi, 256, getRandom());

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

	Image image { juce::Image::PixelFormat::RGB, 250, 250, true };

	fillImageWithRandomPixels (image, getRandom());

	const auto blob         = serializing::imageToBinary (image);
	const auto decodedImage = serializing::imageFromBinary (blob);

	// expect (imagesAreEqual (decodedImage, image));


	beginTest ("Memory block to/from string");

	const auto memStr     = serializing::memoryBlockToString (block);
	const auto memDecoded = serializing::memoryBlockFromString (memStr);

	expect (block == memDecoded);
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
