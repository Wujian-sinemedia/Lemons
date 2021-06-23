
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
    filter.coefs.makeHighPass (samplerate, SampleType (hiPassFreq));
    filter.prepare();

    gate.prepare (2, blocksize, samplerate);
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
    filter.process (audio);
    gate.process (audio, gainReduction);
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
