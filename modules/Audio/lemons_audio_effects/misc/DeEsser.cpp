
namespace lemons::dsp::FX
{
DeEsserParams::DeEsserParams (float threshToUse, int amount)
    : threshDb (threshToUse), deEssAmount (amount)
{
}

template < typename SampleType >
DeEsser< SampleType >::DeEsser()
{
    gate.setAttack (attackMs);
    gate.setRelease (releaseMs);
    gate.setInverted (true);
}

template < typename SampleType >
DeEsser< SampleType >::DeEsser (float threshDB, int deEssAmount)
    : DeEsser()
{
    gate.setThreshold (threshDB);
    setDeEssAmount (deEssAmount);
}

template < typename SampleType >
DeEsser< SampleType >::DeEsser (DeEsserParams params)
    : DeEsser (params.threshDb, params.deEssAmount)
{
}

template < typename SampleType >
void DeEsser< SampleType >::prepare (double samplerate, int blocksize)
{
    filter.coefs.makeHighPass (samplerate, static_cast< SampleType > (hiPassFreq));
    filter.prepare();

    gate.prepare (samplerate, blocksize);
}

template < typename SampleType >
void DeEsser< SampleType >::reset()
{
    filter.reset();
    gate.reset();
}


template < typename SampleType >
void DeEsser< SampleType >::setThresh (float newThresh_dB)
{
    params_.threshDb = newThresh_dB;
    gate.setThreshold (newThresh_dB);
}


template < typename SampleType >
void DeEsser< SampleType >::setDeEssAmount (int newAmount)
{
    jassert (newAmount >= 0 && newAmount <= 100);

    params_.deEssAmount = newAmount;

    const auto a = static_cast< float > (newAmount) * 0.01f;
    gate.setRatio (juce::jmap (a, 0.0f, 1.0f, 1.0f, 10.0f));
}

template < typename SampleType >
SampleType DeEsser< SampleType >::processChannel (int               channel,
                                                  int               numSamples,
                                                  SampleType*       signalToDeEss,
                                                  const SampleType* sidechain)
{
    filter.processChannel (channel, signalToDeEss, numSamples);

    return gate.processChannel (channel, numSamples, signalToDeEss, sidechain);
}

template < typename SampleType >
DeEsserParams DeEsser< SampleType >::getParams() const
{
    return params_;
}

template < typename SampleType >
void DeEsser< SampleType >::setParams (DeEsserParams params)
{
    setThresh (params.threshDb);
    setDeEssAmount (params.deEssAmount);
}

template class DeEsser< float >;
template class DeEsser< double >;

}  // namespace lemons::dsp::FX
