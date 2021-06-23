
namespace bav::dsp::FX
{
template < typename SampleType >
Compressor< SampleType >::Compressor()
{
    update();
}

template < typename SampleType >
void Compressor< SampleType >::setThreshold (float newThresh_dB)
{
    thresholddB = SampleType (newThresh_dB);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::setRatio (float newRatio)
{
    jassert (newRatio >= 1.0f);

    ratio = SampleType (newRatio);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::setAttack (float attackMs)
{
    attackTime = SampleType (attackMs);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::setRelease (float releaseMs)
{
    releaseTime = SampleType (releaseMs);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::prepare (int blocksize, double samplerate, int numChannels)
{
    jassert (samplerate > 0);

    spec.sampleRate       = samplerate;
    spec.maximumBlockSize = juce::uint32 (blocksize);
    spec.numChannels      = juce::uint32 (numChannels);

    envelopeFilter.prepare (spec);

    update();
    reset();
}

template < typename SampleType >
void Compressor< SampleType >::reset()
{
    envelopeFilter.reset();
}

template < typename SampleType >
void Compressor< SampleType >::process (juce::AudioBuffer< SampleType >& signalToCompress,
                                        SampleType*                      gainReduction)
{
    process (signalToCompress, signalToCompress, gainReduction);
}


template < typename SampleType >
void Compressor< SampleType >::process (const juce::AudioBuffer< SampleType >& sidechain,
                                        juce::AudioBuffer< SampleType >&       signalToCompress,
                                        SampleType*                            gainReduction)
{
    const auto numChannels = signalToCompress.getNumChannels();
    const auto numSamples  = signalToCompress.getNumSamples();

    jassert (sidechain.getNumChannels() == numChannels);
    jassert (sidechain.getNumSamples() == numSamples);

    for (int chan = 0; chan < numChannels; ++chan)
    {
        process (chan,
                 numSamples,
                 signalToCompress.getWritePointer (chan),
                 sidechain.getReadPointer (chan),
                 gainReduction);
    }
}


template < typename SampleType >
void Compressor< SampleType >::process (const int         channel,
                                        const int         numSamples,
                                        SampleType*       signalToCompress,
                                        const SampleType* sidechain,
                                        SampleType*       gainReduction)
{
    jassert (numSamples > 0);

    if (sidechain == nullptr) sidechain = signalToCompress;

    SampleType avgGainReduction = 0;
    SampleType gainRedux        = 0;

    for (int s = 0; s < numSamples; ++s)
    {
        *(signalToCompress + s) = processSample (
            channel, signalToCompress[s], sidechain[s], &gainRedux);
        avgGainReduction += gainRedux;
    }

    if (gainReduction != nullptr)
    {
        avgGainReduction *= (1 / numSamples);
        *gainReduction = avgGainReduction;
    }
}


template < typename SampleType >
SampleType Compressor< SampleType >::processSample (int         channel,
                                                    SampleType  inputSample,
                                                    SampleType* gainReduction)
{
    return processSample (channel, inputSample, inputSample, gainReduction);
}


template < typename SampleType >
SampleType Compressor< SampleType >::processSample (int         channel,
                                                    SampleType  inputSample,
                                                    SampleType  sidechainSample,
                                                    SampleType* gainReduction)
{
    auto env = envelopeFilter.processSample (
        channel, sidechainSample);  // Ballistics filter with peak rectifier

    // VCA
    auto gain = (env < threshold) ? SampleType (1.0)
                                  : std::pow (env * thresholdInverse,
                                              ratioInverse - SampleType (1.0));

    if (gainReduction != nullptr)  // report gain reduction, if requested
        *gainReduction = gain;

    return gain * inputSample;
}

template < typename SampleType >
void Compressor< SampleType >::update()
{
    threshold = juce::Decibels::decibelsToGain (
        thresholddB, static_cast< SampleType > (-200.0));
    thresholdInverse = static_cast< SampleType > (1.0) / threshold;
    ratioInverse     = static_cast< SampleType > (1.0) / ratio;

    envelopeFilter.setAttackTime (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}


template class Compressor< float >;
template class Compressor< double >;


template < typename SampleType >
void ReorderableCompressor< SampleType >::fxChain_process (juce::AudioBuffer< SampleType >& audio)
{
    Compressor< SampleType >::process (audio, nullptr);
}

template < typename SampleType >
void ReorderableCompressor< SampleType >::fxChain_prepare (double samplerate, int blocksize)
{
    Compressor< SampleType >::prepare (blocksize, samplerate, 2);
}


template class ReorderableCompressor< float >;
template class ReorderableCompressor< double >;

}  // namespace bav::dsp::FX
