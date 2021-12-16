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


#include <lemons_core/lemons_core.h>

namespace lemons::dsp::buffers
{
template <typename SampleType>
void copy (const AudioBuffer<SampleType>& source, AudioBuffer<SampleType>& dest)
{
    if (&source == &dest)
        return;
    
	dest.clear();

	const auto numSamples = source.getNumSamples();
	jassert (dest.getNumSamples() >= numSamples);

	const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());

	for (int chan = 0; chan < numChannels; ++chan)
		vecops::copy (source.getReadPointer (chan), dest.getWritePointer (chan), numSamples);
}

template void copy (const AudioBuffer<float>&, AudioBuffer<float>&);
template void copy (const AudioBuffer<double>&, AudioBuffer<double>&);

template <typename Type1, typename Type2>
void convert (const AudioBuffer<Type1>& source, AudioBuffer<Type2>& dest)
{
    static_assert (! std::is_same_v<Type1, Type2>,
                   "Converting between two buffers with the same sample type!");
    
	dest.clear();

	const auto numSamples = source.getNumSamples();
	jassert (dest.getNumSamples() >= numSamples);

	const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());

	for (int chan = 0; chan < numChannels; ++chan)
		vecops::convert (dest.getWritePointer (chan), source.getReadPointer (chan), numSamples);
}
template void convert (const AudioBuffer<float>&, AudioBuffer<double>&);
template void convert (const AudioBuffer<double>&, AudioBuffer<float>&);


template <typename SampleType>
AudioBuffer<SampleType> getAliasBuffer (AudioBuffer<SampleType>& bufferToAlias,
                                        int                      startSample,
                                        int                      numSamples,
                                        int                      numChannels,
                                        int                      channelOffset)
{
	if (numChannels == -1)
		numChannels = bufferToAlias.getNumChannels();

	jassert (numChannels > 0);
	jassert (numSamples >= 0);

	jassert (numChannels <= bufferToAlias.getNumChannels());
	jassert (numSamples <= bufferToAlias.getNumSamples());

	return { bufferToAlias.getArrayOfWritePointers() + channelOffset, numChannels, startSample, numSamples };
}
template AudioBuffer<float>  getAliasBuffer (AudioBuffer<float>&, int, int, int, int);
template AudioBuffer<double> getAliasBuffer (AudioBuffer<double>&, int, int, int, int);

}  // namespace lemons::dsp::buffers


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
AudioBufferUtilsTests<FloatType>::AudioBufferUtilsTests()
    : DspTest (getDspTestName<FloatType> ("Audio buffer util functions"))
{
}

template <typename FloatType>
void AudioBufferUtilsTests<FloatType>::resizeAllBuffers (int numSamples, int numChannels)
{
	jassert (numSamples > 0 && numChannels > 0);

	bufferA.setSize (numChannels, numSamples, true, true, true);
	bufferB.setSize (numChannels, numSamples, true, true, true);
}

template <typename FloatType>
void AudioBufferUtilsTests<FloatType>::runTest()
{
	for (const auto numChannels : { 1, 2, 3, 5, 8, 9 })
	{
		beginTest ("Num channels: " + String (numChannels));

		for (const auto numSamples : getTestingBlockSizes())
		{
			const auto blocksizeSubtest = beginSubtest ("Blocksize: " + String (numSamples));

			resizeAllBuffers (numSamples, numChannels);

			using namespace dsp::buffers;

			bufferA.clear();
			bufferB.clear();

			expect (bufferIsSilent (bufferA));
			expect (bufferIsSilent (bufferB));
			expect (buffersAreEqual (bufferA, bufferB));
			expect (noSamplesAreClipping (bufferA));

			bufferA.setSample (0, 1, FloatType (1.001));

			expect (! bufferIsSilent (bufferA));
			expect (! buffersAreEqual (bufferA, bufferB));
			expect (! noSamplesAreClipping (bufferA));

			fillAudioBufferWithRandomNoise (bufferA, getRandom());

			copy (bufferA, bufferB);

			expect (buffersAreEqual (bufferA, bufferB));

			copy (bufferB, bufferA);

			expect (buffersAreEqual (bufferA, bufferB));
			expect (allSamplesAreEqual (bufferA, bufferB, 0, numSamples));

			bufferA.setSample (0, 12, bufferA.getSample (0, 12) + FloatType (0.01));

			expect (! allSamplesAreEqual (bufferA, bufferB, 0, numSamples));
			expect (! buffersAreEqual (bufferA, bufferB));

			copy (bufferB, bufferA);

			expect (buffersAreEqual (bufferA, bufferB));

			const auto halfNumSamples = numSamples / 2;

			const auto aliasA = getAliasBuffer (bufferA, 0, halfNumSamples);

			expect (aliasA.getNumSamples() == halfNumSamples);
			expect (aliasA.getReadPointer (0) == bufferA.getReadPointer (0));
			expect (allSamplesAreEqual (aliasA, bufferB, 0, halfNumSamples));

			const auto aliasB = getAliasBuffer (bufferB, 0, halfNumSamples);

			expect (buffersAreEqual (aliasA, aliasB));

			bufferB.clear();

			expect (! buffersAreEqual (aliasA, aliasB));
			expect (! allSamplesAreZero (aliasA, 0, halfNumSamples));
			expect (! bufferIsSilent (aliasA));

			bufferA.clear();

			expect (buffersAreEqual (aliasA, aliasB));
			expect (allSamplesAreZero (aliasA, 0, halfNumSamples));
			expect (bufferIsSilent (aliasA));
			expect (bufferIsSilent (aliasB));

			if constexpr (std::is_same_v<FloatType, float>)
				runConversionTests<double> (numChannels, numSamples);
			else
				runConversionTests<float> (numChannels, numSamples);

			MidiBuffer midi;
			fillMidiBufferWithRandomEvents (midi, 25, getRandom());
			const auto copiedMidi = makeCopyOfMidiBuffer (midi);
			// expect (midiBuffersAreEqual (midi, copiedMidi));
		}
	}
}

template <typename FloatType>
template <typename OtherFloatType>
void AudioBufferUtilsTests<FloatType>::runConversionTests (int numChannels, int numSamples)
{
	const auto subtest = beginSubtest ("Sample type conversion");

	AudioBuffer<OtherFloatType> otherBuffer { numChannels, numSamples };

	fillAudioBufferWithRandomNoise (bufferA, getRandom());

	const auto checkConvertedBuffers = [&] (const AudioBuffer<FloatType>& bufferToCheck)
	{
		for (int chan = 0; chan < numChannels; ++chan)
		{
			const auto* samplesA = bufferToCheck.getReadPointer (chan);
			const auto* samplesB = otherBuffer.getReadPointer (chan);

			for (int s = 0; s < numSamples; ++s)
				expectWithinAbsoluteError (static_cast<FloatType> (samplesB[s]),
				                           samplesA[s],
				                           static_cast<FloatType> (0.001));
		}
	};

	dsp::buffers::convert (bufferA, otherBuffer);
	checkConvertedBuffers (bufferA);

	dsp::buffers::convert (otherBuffer, bufferB);
	checkConvertedBuffers (bufferB);

	expect (buffersAreReasonablyEqual (bufferA, bufferB));
}

template void AudioBufferUtilsTests<float>::runConversionTests<double> (int, int);
template void AudioBufferUtilsTests<double>::runConversionTests<float> (int, int);

template struct AudioBufferUtilsTests<float>;
template struct AudioBufferUtilsTests<double>;

}  // namespace lemons::tests

#endif
