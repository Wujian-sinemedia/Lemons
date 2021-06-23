
namespace bav::dsp::FX
{
template < typename SampleType >
NoiseGate< SampleType >::NoiseGate()
{
    update();

    RMSFilter.setLevelCalculationType (
        juce::dsp::BallisticsFilterLevelCalculationType::RMS);
    RMSFilter.setAttackTime (static_cast< SampleType > (0.0));
    RMSFilter.setReleaseTime (static_cast< SampleType > (50.0));
}

template < typename SampleType >
void NoiseGate< SampleType >::setInverted (bool gateBehaviorShouldBeInverted)
{
    inverted = gateBehaviorShouldBeInverted;
}


template < typename SampleType >
void NoiseGate< SampleType >::setThreshold (float newThreshold_dB)
{
    thresholddB = static_cast< SampleType > (newThreshold_dB);
    update();
}

template < typename SampleType >
void NoiseGate< SampleType >::setRatio (SampleType newRatio)
{
    jassert (newRatio >= static_cast< SampleType > (1.0));

    ratio = newRatio;
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::setAttack (SampleType newAttack_ms)
{
    attackTime = newAttack_ms;
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::setRelease (SampleType newRelease_ms)
{
    releaseTime = newRelease_ms;
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::prepare (int numChannels, int maxBlocksize, double samplerate)
{
    jassert (samplerate > 0);
    jassert (numChannels > 0);

    spec.sampleRate       = samplerate;
    spec.maximumBlockSize = juce::uint32 (maxBlocksize);
    spec.numChannels      = juce::uint32 (numChannels);

    RMSFilter.prepare (spec);
    envelopeFilter.prepare (spec);

    update();
    reset();
}


template < typename SampleType >
void NoiseGate< SampleType >::reset()
{
    RMSFilter.reset();
    envelopeFilter.reset();
}


template < typename SampleType >
void NoiseGate< SampleType >::process (juce::AudioBuffer< SampleType >& signalToGate,
                                       SampleType*                      gainReduction)
{
    process (signalToGate, signalToGate, gainReduction);
}


template < typename SampleType >
void NoiseGate< SampleType >::process (const juce::AudioBuffer< SampleType >& sidechain,
                                       juce::AudioBuffer< SampleType >&       signalToGate,
                                       SampleType*                            gainReduction)
{
    const auto numChannels = signalToGate.getNumChannels();
    const auto numSamples  = signalToGate.getNumSamples();

    jassert (sidechain.getNumChannels() == numChannels);
    jassert (sidechain.getNumSamples() == numSamples);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        process (channel,
                 numSamples,
                 signalToGate.getWritePointer (channel),
                 sidechain.getReadPointer (channel),
                 gainReduction);
    }
}


template < typename SampleType >
void NoiseGate< SampleType >::process (const int         channel,
                                       const int         numSamples,
                                       SampleType*       signalToGate,
                                       const SampleType* sidechain,
                                       SampleType*       gainReduction)
{
    jassert (numSamples > 0);

    if (sidechain == nullptr) sidechain = signalToGate;

    SampleType avgGainReduction = 0;
    SampleType gainRedux        = 0;

    for (int s = 0; s < numSamples; ++s)
    {
        *(signalToGate + s) =
            processSample (channel, signalToGate[s], sidechain[s], &gainRedux);
        avgGainReduction += gainRedux;
    }

    if (gainReduction != nullptr)
    {
        avgGainReduction *= (1 / numSamples);
        *gainReduction = avgGainReduction;
    }
}


template < typename SampleType >
SampleType NoiseGate< SampleType >::processSample (const int        channel,
                                                   const SampleType sampleToGate,
                                                   SampleType*      gainReduction)
{
    return processSample (channel, sampleToGate, sampleToGate, gainReduction);
}


template < typename SampleType >
SampleType NoiseGate< SampleType >::processSample (const int        channel,
                                                   const SampleType sampleToGate,
                                                   const SampleType sidechainValue,
                                                   SampleType*      gainReduction)
{
    auto env = RMSFilter.processSample (channel,
                                        sidechainValue);  // RMS ballistics filter

    env = envelopeFilter.processSample (channel, env);  // Ballistics filter

    // VCA
    SampleType gain;

    if (inverted)
    {
        gain = (env < threshold)
                 ? static_cast< SampleType > (1.0)
                 : std::pow (env * thresholdInverse,
                             currentRatio - static_cast< SampleType > (1.0));
    }
    else
    {
        gain = (env > threshold)
                 ? static_cast< SampleType > (1.0)
                 : std::pow (env * thresholdInverse,
                             currentRatio - static_cast< SampleType > (1.0));
    }

    if (gainReduction != nullptr)  // report gain reduction, if requested
        *gainReduction = gain;

    return gain * sampleToGate;
}

template < typename SampleType >
void NoiseGate< SampleType >::update()
{
    threshold = juce::Decibels::decibelsToGain (
        thresholddB, static_cast< SampleType > (-200.0));
    thresholdInverse = static_cast< SampleType > (1.0) / threshold;
    currentRatio     = ratio;

    envelopeFilter.setAttackTime (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}


template class NoiseGate< float >;
template class NoiseGate< double >;


template < typename SampleType >
void ReorderableNoiseGate< SampleType >::fxChain_process (juce::AudioBuffer< SampleType >& audio)
{
    NoiseGate< SampleType >::process (audio, nullptr);
}

template < typename SampleType >
void ReorderableNoiseGate< SampleType >::fxChain_prepare (double samplerate, int blocksize)
{
    NoiseGate< SampleType >::prepare (2, blocksize, samplerate);
}


template class ReorderableNoiseGate< float >;
template class ReorderableNoiseGate< double >;

}  // namespace bav::dsp::FX
