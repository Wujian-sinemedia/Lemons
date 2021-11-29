#include <lemons_core/lemons_core.h>

namespace lemons::dsp::buffers
{
template <typename SampleType>
void copy (const AudioBuffer<SampleType>& source, AudioBuffer<SampleType>& dest)
{
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
    : DspTest ("AudioBufferUtilsTests")
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
	constexpr auto numChannels = 3;

	for (const auto numSamples : getTestingBlockSizes())
	{
		logBlocksizeMessage (numSamples);

		resizeAllBuffers (numSamples, numChannels);

		using namespace dsp::buffers;


		beginTest ("copy()");

		bufferA.clear();
		bufferB.clear();

		osc.setFrequency (440.f, 44100.);
		osc.getSamples (bufferA);

		copy (bufferA, bufferB);

		expect (buffersAreEqual (bufferA, bufferB));

		copy (bufferB, bufferA);

		expect (buffersAreEqual (bufferA, bufferB));


		beginTest ("allSamplesAreEqual() and buffersAreEqual()");

		expect (allSamplesAreEqual (bufferA, bufferB, 0, numSamples));

		bufferA.setSample (0, 12, bufferA.getSample (0, 12) + FloatType (0.01));

		expect (! allSamplesAreEqual (bufferA, bufferB, 0, numSamples));

		expect (! buffersAreEqual (bufferA, bufferB));


		beginTest ("copy() again");

		copy (bufferB, bufferA);

		expect (buffersAreEqual (bufferA, bufferB));


		beginTest ("getAliasBuffer()");

		const auto halfNumSamples = numSamples / 2;

		const auto aliasA = getAliasBuffer (bufferA, 0, halfNumSamples);

		expect (aliasA.getNumSamples() == halfNumSamples);

		expect (aliasA.getReadPointer (0) == bufferA.getReadPointer (0));

		expect (allSamplesAreEqual (aliasA, bufferB, 0, halfNumSamples));

		const auto aliasB = getAliasBuffer (bufferB, 0, halfNumSamples);

		expect (buffersAreEqual (aliasA, aliasB));

		bufferB.clear();

		expect (! buffersAreEqual (aliasA, aliasB));


		beginTest ("allSamplesAreZero()");

		expect (! allSamplesAreZero (aliasA, 0, halfNumSamples));

		bufferA.clear();

		expect (buffersAreEqual (aliasA, aliasB));

		expect (allSamplesAreZero (aliasA, 0, halfNumSamples));

		//    beginTest ("convert()");
	}
}

template struct AudioBufferUtilsTests<float>;
template struct AudioBufferUtilsTests<double>;

}  // namespace lemons::tests

#endif
