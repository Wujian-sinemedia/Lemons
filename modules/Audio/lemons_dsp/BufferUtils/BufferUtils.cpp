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


template <typename FloatType>
inline bool allSamplesAreEqual (const juce::AudioBuffer<FloatType>& buffer1,
                                const juce::AudioBuffer<FloatType>& buffer2,
                                int startIndex1, int numSamples, int startIndex2,
                                int channel1, int channel2)
{
	if (startIndex2 < 0) startIndex2 = startIndex1;
	if (channel2 < 0) channel2 = channel1;

	jassert (startIndex1 + numSamples <= buffer1.getNumSamples());
	jassert (startIndex2 + numSamples <= buffer2.getNumSamples());
	jassert (channel1 < buffer1.getNumChannels());
	jassert (channel2 < buffer2.getNumChannels());

	for (int i1 = startIndex1, i2 = startIndex2;
	     i1 < startIndex1 + numSamples && i2 < startIndex2 + numSamples;
	     ++i1, ++i2)
	{
		if (buffer1.getSample (channel1, i1) != buffer2.getSample (channel2, i2))
			return false;
	}

	return true;
}

template bool allSamplesAreEqual (const AudioBuffer<float>&, const AudioBuffer<float>&, int, int, int, int, int);
template bool allSamplesAreEqual (const AudioBuffer<double>&, const AudioBuffer<double>&, int, int, int, int, int);


template <typename FloatType>
bool buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
                      const AudioBuffer<FloatType>& buffer2)
{
	jassert (buffer1.getNumChannels() == buffer2.getNumChannels());

	const auto numSamples = buffer1.getNumSamples();
	jassert (numSamples == buffer2.getNumSamples());

	for (int chan = 0; chan < buffer1.getNumChannels(); ++chan)
		if (! allSamplesAreEqual (buffer1, buffer2, 0, numSamples, 0, chan, chan))
			return false;

	return true;
}

template bool buffersAreEqual (const AudioBuffer<float>&, const AudioBuffer<float>&);
template bool buffersAreEqual (const AudioBuffer<double>&, const AudioBuffer<double>&);


template <typename FloatType>
bool allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
                        int startIndex, int numSamples, int channel)
{
	jassert (startIndex + numSamples <= buffer.getNumSamples());
	jassert (channel < buffer.getNumChannels());

	for (int i = startIndex; i < startIndex + numSamples; ++i)
		if (buffer.getSample (channel, i) != FloatType (0))
			return false;

	return true;
}

template bool allSamplesAreZero (const AudioBuffer<float>&, int, int, int);
template bool allSamplesAreZero (const AudioBuffer<double>&, int, int, int);

}  // namespace lemons::dsp::buffers


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
AudioBufferUtilsTests<FloatType>::AudioBufferUtilsTests()
    : juce::UnitTest ("AudioBufferUtilsTests", "DSP")
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
	constexpr auto numSamples = 512;

	resizeAllBuffers (numSamples, 3);

	using namespace dsp::buffers;

	beginTest ("copy()");

	osc.setFrequency (440.f, 44100.);
	osc.getSamples (bufferA);
	bufferB.clear();

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

	constexpr auto halfNumSamples = numSamples / 2;

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

template struct AudioBufferUtilsTests<float>;
template struct AudioBufferUtilsTests<double>;

}  // namespace lemons::tests

#endif
