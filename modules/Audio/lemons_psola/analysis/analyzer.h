#pragma once

#include <lemons_dsp/lemons_dsp.h>


namespace lemons::dsp::psola
{

template<typename SampleType>
class Shifter;

template<typename SampleType>
class Analyzer final
{
public:
    
    explicit Analyzer (int minFreqHz = 60);
    
    void analyzeInput (const AudioBuffer<SampleType>& inputAudio);
    
    void analyzeInput (const SampleType* inputAudio, int numSamples);
    
    [[nodiscard]] int getLatencySamples() const noexcept;
    
    int setSamplerate (double newSamplerate);
    
    int setMinInputFreq (int minFreqHz);
    
private:
    friend class Shifter<SampleType>;
    
    inline int latencyChanged();
    
    PitchDetector<SampleType> pitchDetector;
    
    float currentPeriod { 0.f };
    
    AudioBuffer<SampleType> storage { 1, 512 };
};

}
