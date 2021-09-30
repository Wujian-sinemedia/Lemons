
namespace lemons::dsp::FX
{
NoiseGateParams::NoiseGateParams (float threshToUse, float ratioToUse, float attackTime, float releaseTime, bool shouldBeInverted)
    : threshDB (threshToUse), ratio (ratioToUse), attackMs (attackTime), releaseMs (releaseTime), inverted (shouldBeInverted)
{
}

template < typename SampleType >
NoiseGate< SampleType >::NoiseGate()
{
    RMSFilter.setLevelCalculationType (juce::dsp::BallisticsFilterLevelCalculationType::RMS);
    RMSFilter.setAttackTime (static_cast< SampleType > (0.0));
    RMSFilter.setReleaseTime (static_cast< SampleType > (50.0));

    spec.numChannels = 2;
    update();
}

template < typename SampleType >
NoiseGate< SampleType >::NoiseGate (float threshDB, float ratioToUse, float attackMs, float releaseMs, bool shouldBeInverted)
{
    RMSFilter.setLevelCalculationType (juce::dsp::BallisticsFilterLevelCalculationType::RMS);
    RMSFilter.setAttackTime (static_cast< SampleType > (0.0));
    RMSFilter.setReleaseTime (static_cast< SampleType > (50.0));

    thresholddB = static_cast< SampleType > (threshDB);
    ratio       = static_cast< SampleType > (ratioToUse);
    attackTime  = static_cast< SampleType > (attackMs);
    releaseTime = static_cast< SampleType > (releaseMs);

    spec.numChannels = 2;
    update();
}

template < typename SampleType >
NoiseGate< SampleType >::NoiseGate (NoiseGateParams params)
    : NoiseGate (params.threshDB, params.ratio, params.attackMs, params.releaseMs, params.inverted)
{
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
void NoiseGate< SampleType >::setRatio (float newRatio)
{
    jassert (newRatio >= 0.f);

    ratio = static_cast< SampleType > (newRatio);
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::setAttack (float newAttack_ms)
{
    attackTime = static_cast< SampleType > (newAttack_ms);
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::setRelease (float newRelease_ms)
{
    releaseTime = static_cast< SampleType > (newRelease_ms);
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (samplerate > 0);

    spec.sampleRate       = samplerate;
    spec.maximumBlockSize = static_cast< juce::uint32 > (blocksize);

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
SampleType NoiseGate< SampleType >::processChannel (int               channel,
                                                    int               numSamples,
                                                    SampleType*       signalToGate,
                                                    const SampleType* sidechain)
{
    if (numSamples == 0) return (SampleType) 0;

    SampleType avgGainReduction = 0;
    SampleType gainRedux        = 0;

    for (int s = 0; s < numSamples; ++s)
    {
        signalToGate[s] = processSample (channel, signalToGate[s], sidechain[s], &gainRedux);
        avgGainReduction += gainRedux;
    }

    return avgGainReduction * ((SampleType) 1 / (SampleType) numSamples);
}


template < typename SampleType >
SampleType NoiseGate< SampleType >::processSample (int         channel,
                                                   SampleType  sampleToGate,
                                                   SampleType  sidechainValue,
                                                   SampleType* gainReduction)
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

template < typename SampleType >
NoiseGateParams NoiseGate< SampleType >::getParams() const
{
    return {static_cast< float > (thresholddB),
            static_cast< float > (ratio),
            static_cast< float > (attackTime),
            static_cast< float > (releaseTime),
            inverted};
}

template < typename SampleType >
void NoiseGate< SampleType >::setParams (NoiseGateParams params)
{
    inverted    = params.inverted;
    thresholddB = static_cast< SampleType > (params.threshDB);
    ratio       = static_cast< SampleType > (params.ratio);
    attackTime  = static_cast< SampleType > (params.attackMs);
    releaseTime = static_cast< SampleType > (params.releaseMs);
    update();
}


template class NoiseGate< float >;
template class NoiseGate< double >;

}  // namespace lemons::dsp::FX
