
namespace bav::dsp::FX
{
template < typename SampleType >
DeEsser< SampleType >::DeEsser()
{
    gate.setAttack (attackMs);
    gate.setRelease (releaseMs);
    gate.setInverted (true);
}

template < typename SampleType >
void DeEsser< SampleType >::prepare (int blocksize, double samplerate)
{
    hiPass.coefficients =
        juce::dsp::IIR::Coefficients< SampleType >::makeHighPass (
            samplerate, SampleType (hiPassFreq));
    gate.prepare (2, blocksize, samplerate);
}

template < typename SampleType >
void DeEsser< SampleType >::reset()
{
    hiPass.reset();
    gate.reset();
}


template < typename SampleType >
void DeEsser< SampleType >::setThresh (float newThresh_dB)
{
    gate.setThreshold (newThresh_dB);
}


template < typename SampleType >
void DeEsser< SampleType >::setDeEssAmount (int newAmount)
{
    jassert (newAmount >= 0 && newAmount <= 100);
    auto a = float (newAmount) * 0.01f;
    gate.setRatio (SampleType (juce::jmap (a, 0.0f, 1.0f, 1.0f, 10.0f)));
}


template < typename SampleType >
void DeEsser< SampleType >::process (juce::AudioBuffer< SampleType >& audio,
                                     SampleType*                      gainReduction)
{
    const auto numSamples = audio.getNumSamples();

    for (int chan = 0; chan < std::min (2, audio.getNumChannels()); ++chan)
        process (chan, audio.getWritePointer (chan), numSamples, gainReduction);
}


template < typename SampleType >
void DeEsser< SampleType >::process (const int   channel,
                                     SampleType* signalToDeEss,
                                     const int   numSamples,
                                     SampleType* gainReduction)
{
    SampleType avgGainReduction = 0;
    SampleType gainRedux        = 0;

    for (int s = 0; s < numSamples; ++s)
    {
        *(signalToDeEss + s) =
            processSample (channel, signalToDeEss[s], &gainRedux);
        avgGainReduction += gainRedux;
    }

    if (gainReduction != nullptr)
    {
        avgGainReduction *= (1 / numSamples);
        *gainReduction = avgGainReduction;
    }
}


template < typename SampleType >
SampleType DeEsser< SampleType >::processSample (const int        channel,
                                                 const SampleType inputSample,
                                                 SampleType*      gainReduction)
{
    return gate.processSample (
        channel, inputSample, hiPass.processSample (inputSample), gainReduction);
}

template class DeEsser< float >;
template class DeEsser< double >;


template < typename SampleType >
void ReorderableDeEsser< SampleType >::fxChain_process (juce::AudioBuffer< SampleType >& audio)
{
    DeEsser< SampleType >::process (audio, nullptr);
}

template < typename SampleType >
void ReorderableDeEsser< SampleType >::fxChain_prepare (double samplerate, int blocksize)
{
    DeEsser< SampleType >::prepare (blocksize, samplerate);
}


template class ReorderableDeEsser< float >;
template class ReorderableDeEsser< double >;

}  // namespace bav::dsp::FX
