namespace lemons::dsp
{

template<typename SampleType>
SampleType WhiteNoiseGenerator<SampleType>::getNextSample()
{
    return rand.nextFloat() * 0.25f - 0.125f;
}

template<typename SampleType>
void WhiteNoiseGenerator<SampleType>::getSamples (SampleType* output, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        output[i] = getNextSample();
}

template<typename SampleType>
void WhiteNoiseGenerator<SampleType>::getSamples (AudioBuffer<SampleType>& output, int channel)
{
    jassert (channel < output.getNumChannels());
    getSamples (output.getWritePointer(channel), output.getNumSamples());
}

template class WhiteNoiseGenerator<float>;
template class WhiteNoiseGenerator<double>;

}

