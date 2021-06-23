namespace bav::dsp::FX
{
template < typename SampleType >
void Delay< SampleType >::setDelay (int delayInSamples)
{
    delay.setDelay (SampleType (delayInSamples));
}

template < typename SampleType >
void Delay< SampleType >::prepare (int blocksize, double samplerate, int numChannels)
{
    jassert (samplerate > 0);

    spec.sampleRate       = samplerate;
    spec.maximumBlockSize = juce::uint32 (blocksize);
    spec.numChannels      = juce::uint32 (numChannels);

    delay.prepare (spec);

    dryGain.reset (blocksize);
    wetGain.reset (blocksize);
}

template < typename SampleType >
void Delay< SampleType >::reset()
{
    delay.reset();
    const auto blocksize = int (spec.maximumBlockSize);
    dryGain.reset (blocksize);
    wetGain.reset (blocksize);
}

template < typename SampleType >
void Delay< SampleType >::setDryWet (int wetMixPercent)
{
    const auto wet = wetMixPercent * 0.01f;
    wetGain.set (wet);
    dryGain.set (1.0f - wet);
}


template < typename SampleType >
void Delay< SampleType >::pushSample (int channel, SampleType sample)
{
    delay.pushSample (channel, sample);
}


template < typename SampleType >
SampleType Delay< SampleType >::popSample (int channel, SampleType* delayLevel)
{
    const auto delaySamps = delay.getDelay();

    const auto drySample = delay.popSample (channel, SampleType (0), false)
                         * dryGain.getNextValue();
    const auto wetSample =
        delay.popSample (channel, delaySamps, true) * wetGain.getNextValue();

    if (delayLevel != nullptr) *delayLevel = std::abs (wetSample);

    return drySample + wetSample;
}


template < typename SampleType >
void Delay< SampleType >::process (int         channelNum,
                                   SampleType* samples,
                                   int         numSamples,
                                   SampleType* delayLevel)
{
    jassert (numSamples > 0);

    for (int i = 0; i < numSamples; ++i)
        pushSample (channelNum, samples[i]);

    auto avgMag = SampleType (0.0);

    for (int i = 0; i < numSamples; ++i)
    {
        SampleType mag;
        *(samples + i) = popSample (channelNum, &mag);
        avgMag += mag;
    }

    if (delayLevel != nullptr)
    {
        avgMag /= numSamples;
        *delayLevel = avgMag;
    }
}


template < typename SampleType >
void Delay< SampleType >::process (juce::AudioBuffer< SampleType >& audio,
                                   SampleType*                      delayLevel)
{
    const auto numSamples  = audio.getNumSamples();
    const auto numChannels = audio.getNumChannels();

    jassert (numChannels > 0);

    auto avgMag = SampleType (0.0);

    for (int i = 0; i < numChannels; ++i)
    {
        SampleType mag;
        process (i, audio.getWritePointer (i), numSamples, &mag);
        avgMag += mag;
    }

    if (delayLevel != nullptr)
    {
        avgMag /= numChannels;
        *delayLevel = avgMag;
    }
}

template class Delay< float >;
template class Delay< double >;


template < typename SampleType >
void ReorderableDelay< SampleType >::fxChain_process (juce::AudioBuffer< SampleType >& audio)
{
    Delay< SampleType >::process (audio);
}

template < typename SampleType >
void ReorderableDelay< SampleType >::fxChain_prepare (double samplerate, int blocksize)
{
    Delay< SampleType >::prepare (blocksize, samplerate, 2);
}


template class ReorderableDelay< float >;
template class ReorderableDelay< double >;

}  // namespace bav::dsp::FX
