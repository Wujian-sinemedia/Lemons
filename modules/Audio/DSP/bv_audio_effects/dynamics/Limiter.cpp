
namespace bav::dsp::FX
{
template < typename SampleType >
void Limiter< SampleType >::setThreshold (float thresh_dB)
{
    thresholddB = thresh_dB;
    update();
}

template < typename SampleType >
void Limiter< SampleType >::setRelease (float release_ms)
{
    releaseTime = release_ms;
    update();
}

template < typename SampleType >
void Limiter< SampleType >::prepare (int blocksize, double samplerate, int numChannels)
{
    jassert (samplerate > 0);
    jassert (numChannels > 0);

    sampleRate = samplerate;

    firstStageCompressor.prepare (blocksize, samplerate, numChannels);
    secondStageCompressor.prepare (blocksize, samplerate, numChannels);

    update();
    reset();
}

template < typename SampleType >
void Limiter< SampleType >::reset()
{
    firstStageCompressor.reset();
    secondStageCompressor.reset();

    outputVolume.reset (sampleRate, 0.001);
}


template < typename SampleType >
void Limiter< SampleType >::process (juce::AudioBuffer< SampleType >& signalToLimit,
                                     SampleType*                      gainReduction)
{
    process (signalToLimit, signalToLimit, gainReduction);
}


template < typename SampleType >
void Limiter< SampleType >::process (const juce::AudioBuffer< SampleType >& sidechain,
                                     juce::AudioBuffer< SampleType >&       signalToLimit,
                                     SampleType*                            gainReduction)
{
    const auto numChannels = signalToLimit.getNumChannels();
    const auto numSamples  = signalToLimit.getNumSamples();

    jassert (sidechain.getNumChannels() == numChannels);
    jassert (sidechain.getNumSamples() == numSamples);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        process (channel,
                 numSamples,
                 signalToLimit.getWritePointer (channel),
                 sidechain.getReadPointer (channel),
                 gainReduction);
    }
}


template < typename SampleType >
void Limiter< SampleType >::process (const int         channel,
                                     const int         numSamples,
                                     SampleType*       signalToLimit,
                                     const SampleType* sidechain,
                                     SampleType*       gainReduction)
{
    if (sidechain == nullptr) sidechain = signalToLimit;

    auto getMagnitude = [] (SampleType* signal, int numSamps)
    {
        auto r = juce::FloatVectorOperations::findMinAndMax (signal, numSamps);
        return juce::jmax (r.getStart(), -r.getStart(), r.getEnd(), -r.getEnd());
    };

    const auto levelBefore = getMagnitude (signalToLimit, numSamples);

    for (int s = 0; s < numSamples; ++s)
    {
        const auto sc = sidechain[s];

        *(signalToLimit + s) =
            firstStageCompressor.processSample (channel, signalToLimit[s], sc);
        *(signalToLimit + s) =
            secondStageCompressor.processSample (channel, signalToLimit[s], sc);
        *(signalToLimit + s) = signalToLimit[s] * outputVolume.getNextValue();
    }

    juce::FloatVectorOperations::clip (signalToLimit,
                                       signalToLimit,
                                       SampleType (-1.0),
                                       SampleType (1.0),
                                       numSamples);

    if (gainReduction != nullptr)
    {
        const auto levelAfter = getMagnitude (signalToLimit, numSamples);

        *gainReduction = juce::jlimit (
            SampleType (0.0), SampleType (1.0), levelAfter - levelBefore);
    }
}

template < typename SampleType >
void Limiter< SampleType >::update()
{
    firstStageCompressor.setThreshold (-10.0f);
    firstStageCompressor.setRatio (4.0f);
    firstStageCompressor.setAttack (2.0f);
    firstStageCompressor.setRelease (200.0f);

    secondStageCompressor.setThreshold (thresholddB);
    secondStageCompressor.setRatio (1000.0f);
    secondStageCompressor.setAttack (0.001f);
    secondStageCompressor.setRelease (releaseTime);

    auto ratioInverse = (SampleType) (1.0 / 4.0);

    auto gain = (SampleType) std::pow (10.0, 10.0 * (1.0 - ratioInverse) / 40.0);
    gain *= juce::Decibels::decibelsToGain (-thresholddB, -100.0f);

    outputVolume.setTargetValue (gain);
}


template class Limiter< float >;
template class Limiter< double >;


template < typename SampleType >
void ReorderableLimiter< SampleType >::fxChain_process (juce::AudioBuffer< SampleType >& audio)
{
    Limiter< SampleType >::process (audio);
}

template < typename SampleType >
void ReorderableLimiter< SampleType >::fxChain_prepare (double samplerate, int blocksize)
{
    Limiter< SampleType >::prepare (blocksize, samplerate, 2);
}


template class ReorderableLimiter< float >;
template class ReorderableLimiter< double >;

}  // namespace bav::dsp::FX
