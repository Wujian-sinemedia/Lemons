
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

    const auto pitchInHz = pitchDetector.detectPitch (samples, numSamples);

    const auto period = pitchInHz > 0.f ? math::periodInSamples (samplerate, pitchInHz)
                                        : getNextUnpitchedPeriod();

    grains.analyzeInput (samples, numSamples, period);

    currentPeriod = period;

    broadcaster.trigger();
}

template < typename SampleType >
int PsolaAnalyzer< SampleType >::getStartOfClosestGrain (int sampleIndex) const
{
    return grains.getStartOfClosestGrain (sampleIndex);
}

template < typename SampleType >
int PsolaAnalyzer< SampleType >::getPeriod() const
{
    return currentPeriod;
}

template < typename SampleType >
int PsolaAnalyzer< SampleType >::getGrainLength() const
{
    return currentPeriod * 2;
}

template < typename SampleType >
int PsolaAnalyzer< SampleType >::getLatencySamples() const
{
    return pitchDetector.getLatencySamples();
}

template < typename SampleType >
int PsolaAnalyzer< SampleType >::getNextUnpitchedPeriod()
{
    return rand.nextInt (pitchDetector.getCurrentLegalPeriodRange());
}

template < typename SampleType >
const psola::AnalysisGrainStorage< SampleType >& PsolaAnalyzer< SampleType >::getStorage() const
{
    return grains.getStorage();
}

template < typename SampleType >
events::Broadcaster& PsolaAnalyzer< SampleType >::getBroadcaster()
{
    return broadcaster;
}


template class PsolaAnalyzer< float >;
template class PsolaAnalyzer< double >;

}  // namespace bav::dsp
