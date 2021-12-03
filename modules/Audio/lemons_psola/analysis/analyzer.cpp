#include <lemons_core/lemons_core.h>

namespace lemons::dsp::psola
{

template<typename SampleType>
Analyzer<SampleType>::Analyzer (int minFreqHz)
{
    setMinInputFreq (minFreqHz);
}

template<typename SampleType>
void Analyzer<SampleType>::analyzeInput (const AudioBuffer<SampleType>& inputAudio)
{
    analyzeInput (inputAudio.getReadPointer(0), inputAudio.getNumSamples());
}

template<typename SampleType>
void Analyzer<SampleType>::analyzeInput (const SampleType* inputAudio, int numSamples)
{
    jassert (numSamples >= getLatencySamples());
    
    const auto detectedPeriod = pitchDetector.detectPeriod (inputAudio, numSamples);
    
    const auto isPitched = detectedPeriod > 0.f;
    
    // if frame is unpitched, assign a constant period value to use...
    
    currentPeriod = detectedPeriod;
    
    const auto& indices = peakFinder.findPeaks (inputAudio, numSamples, detectedPeriod);
}


template<typename SampleType>
[[nodiscard]] int Analyzer<SampleType>::getLatencySamples() const noexcept
{
    return pitchDetector.getLatencySamples();
}

template<typename SampleType>
int Analyzer<SampleType>::setSamplerate (double newSamplerate)
{
    jassert (newSamplerate > 0.);
    
    pitchDetector.setSamplerate (newSamplerate);
    
    return latencyChanged();
}

template<typename SampleType>
int Analyzer<SampleType>::setMinInputFreq (int minFreqHz)
{
    jassert (minFreqHz > 0);
    
    pitchDetector.setMinHz (minFreqHz);
    
    return latencyChanged();
}

template<typename SampleType>
inline int Analyzer<SampleType>::latencyChanged()
{
    const auto latency = getLatencySamples();
    
    peakFinder.prepare (latency);
    
    return latency;
}

template class Analyzer<float>;
template class Analyzer<double>;

}
