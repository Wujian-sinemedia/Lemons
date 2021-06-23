
namespace bav::dsp::FX
{
template < typename SampleType >
void MonoToStereoPanner< SampleType >::prepare (int blocksize)
{
    left.prepare (blocksize);
    right.prepare (blocksize);
}

template < typename SampleType >
void MonoToStereoPanner< SampleType >::reset()
{
    left.reset();
    right.reset();
}


template < typename SampleType >
void MonoToStereoPanner< SampleType >::process (const SampleType* input,
                                                SampleType*       leftOut,
                                                SampleType*       rightOut,
                                                int               numSamples)
{
    left.setGain (PannerBase::getLeftGain());
    right.setGain (PannerBase::getRightGain());

    vecops::copy (input, leftOut, numSamples);
    vecops::copy (input, rightOut, numSamples);

    left.process (leftOut, numSamples, 0);
    right.process (rightOut, numSamples, 0);
}


template < typename SampleType >
void MonoToStereoPanner< SampleType >::process (const juce::AudioBuffer< SampleType >& monoInput,
                                                juce::AudioBuffer< SampleType >&       stereoOutput)
{
    stereoOutput.clear();
    jassert (stereoOutput.getNumChannels() >= 2);
    jassert (monoInput.getNumSamples() == stereoOutput.getNumSamples());

    process (monoInput.getReadPointer (0),
             stereoOutput.getWritePointer (0),
             stereoOutput.getWritePointer (1),
             stereoOutput.getNumSamples());
}

template class MonoToStereoPanner< float >;
template class MonoToStereoPanner< double >;

}  // namespace bav::dsp::FX
