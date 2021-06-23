
namespace bav::dsp::FX
{
template < typename SampleType, size_t channels >
SmoothedGain< SampleType, channels >::SmoothedGain()
{
    for (size_t i = 0; i < channels; ++i)
        smoothers.add (new Smoother());
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::setGain (float gain)
{
    const auto newTarget = static_cast< SampleType > (std::max (gain, 0.0001f));

    for (auto* smoother : smoothers)
        smoother->setTargetValue (newTarget);
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::prepare (int blocksize)
{
    lastBlocksize = blocksize;

    reset();
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::reset()
{
    for (auto* smoother : smoothers)
        smoother->reset (lastBlocksize);
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::skipSamples (int numSamples)
{
    for (auto* smoother : smoothers)
        for (int s = 0; s < numSamples; ++s)
            smoother->getNextValue();
}


template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::process (SampleType* samples, int numSamples, int channel, float newGain)
{
    setGain (newGain);
    process (samples, numSamples, channel);
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::process (SampleType* samples, int numSamples, int channel)
{
    jassert (static_cast< size_t > (channel) < channels);
    smoothers[channel]->applyGain (samples, numSamples);
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::process (juce::AudioBuffer< SampleType >& audio, float newGain)
{
    setGain (newGain);
    process (audio);
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::process (juce::AudioBuffer< SampleType >& audio)
{
    const auto numSamples = audio.getNumSamples();

    for (int chan = 0;
         chan < std::min (static_cast< int > (channels), audio.getNumChannels());
         ++chan)
    {
        process (audio.getWritePointer (chan), numSamples, chan);
    }
}


template class SmoothedGain< float, 1 >;
template class SmoothedGain< double, 1 >;
template class SmoothedGain< float, 2 >;
template class SmoothedGain< double, 2 >;


template < typename SampleType >
void ReorderableSmoothedGain< SampleType >::fxChain_process (juce::AudioBuffer< SampleType >& audio)
{
    SmoothedGain< SampleType >::process (audio);
}

template < typename SampleType >
void ReorderableSmoothedGain< SampleType >::fxChain_prepare (double, int blocksize)
{
    SmoothedGain< SampleType >::prepare (blocksize);
}

template class ReorderableSmoothedGain< float >;
template class ReorderableSmoothedGain< double >;

}  // namespace bav::dsp::FX
