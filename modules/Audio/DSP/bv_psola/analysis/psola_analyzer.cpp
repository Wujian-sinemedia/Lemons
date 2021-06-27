
namespace bav::dsp
{
template < typename SampleType >
void PsolaAnalyzer< SampleType >::prepare (double sampleRate, int blocksize)
{
    samplerate = sampleRate;
    pitchDetector.setSamplerate (sampleRate);
    grains.prepare (blocksize);
}

template < typename SampleType >
void PsolaAnalyzer< SampleType >::analyzeInput (const juce::AudioBuffer< SampleType >& audio)
{
    analyzeInput (audio.getReadPointer (0), audio.getNumSamples());
}

template < typename SampleType >
void PsolaAnalyzer< SampleType >::analyzeInput (const SampleType* samples, int numSamples)
{
    jassert (samplerate > 0);

    const auto pitchInHz      = pitchDetector.detectPitch (samples, numSamples);
    const auto frameIsPitched = pitchInHz > 0.f;

    const auto period = frameIsPitched ? math::periodInSamples (samplerate, pitchInHz)
                                       : getNextUnpitchedPeriod();

    grains.analyzeInput (samples, numSamples, period);

    currentPeriod = period;
}

template < typename SampleType >
int PsolaAnalyzer< SampleType >::getNextUnpitchedPeriod()
{
    return rand.nextInt (pitchDetector.getCurrentLegalPeriodRange());
}


template class PsolaAnalyzer< float >;
template class PsolaAnalyzer< double >;

}  // namespace bav::dsp
