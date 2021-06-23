
namespace bav::dsp::FX
{
template < typename SampleType >
void StereoPanner< SampleType >::prepare (int blocksize)
{
    left.prepare (blocksize);
    right.prepare (blocksize);
}

template < typename SampleType >
void StereoPanner< SampleType >::reset()
{
    left.reset();
    right.reset();
}


template < typename SampleType >
void StereoPanner< SampleType >::process (const SampleType* leftIn,
                                          const SampleType* rightIn,
                                          SampleType*       leftOut,
                                          SampleType*       rightOut,
                                          int               numSamples)
{
    left.setGain (PannerBase::getLeftGain());
    right.setGain (PannerBase::getRightGain());

    vecops::copy (leftIn, leftOut, numSamples);
    vecops::copy (rightIn, rightOut, numSamples);

    left.process (leftOut, numSamples, 0);
    right.process (rightOut, numSamples, 0);
}


template < typename SampleType >
void StereoPanner< SampleType >::process (const juce::AudioBuffer< SampleType >& stereoInput,
                                          juce::AudioBuffer< SampleType >&       stereoOutput)
{
    stereoOutput.clear();
    jassert (stereoInput.getNumChannels() >= 2
             && stereoOutput.getNumChannels() >= 2);
    jassert (stereoInput.getNumSamples() == stereoOutput.getNumSamples());

    process (stereoInput.getReadPointer (0),
             stereoInput.getReadPointer (1),
             stereoOutput.getWritePointer (0),
             stereoOutput.getWritePointer (1),
             stereoOutput.getNumSamples());
}

template class StereoPanner< float >;
template class StereoPanner< double >;


template < typename SampleType >
void ReorderableStereoPanner< SampleType >::fxChain_process (juce::AudioBuffer< SampleType >& audio)
{
    StereoPanner< SampleType >::process (audio, audio);
}

template < typename SampleType >
void ReorderableStereoPanner< SampleType >::fxChain_prepare (double, int blocksize)
{
    StereoPanner< SampleType >::prepare (blocksize);
}

template class ReorderableStereoPanner< float >;
template class ReorderableStereoPanner< double >;

}  // namespace bav::dsp::FX
