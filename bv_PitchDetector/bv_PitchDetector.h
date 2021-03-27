/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_PitchDetector
 vendor:             Ben Vining
 version:            0.0.1
 name:               Pitch detector
 description:        ASDF-based pitch detector
 dependencies:       bv_SharedCode
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once

#include "bv_SharedCode/bv_SharedCode.h"


namespace bav
{
    

template<typename SampleType>
class PitchDetector
{
    
    using AudioBuffer = juce::AudioBuffer<SampleType>;
    
    
public:
    
    PitchDetector();
    
    ~PitchDetector();
    
    void initialize();
    
    void releaseResources();
    
    float detectPitch (const AudioBuffer& inputAudio);
    
    int getLatencySamples() const noexcept { return 2 * maxPeriod; }
    
    void setHzRange (const int newMinHz, const int newMaxHz);
    
    void setConfidenceThresh (const SampleType newThresh) { confidenceThresh = newThresh; }
    
    void setSamplerate (const double newSamplerate);
    
    
private:
    
    int minHz, maxHz;
    int minPeriod, maxPeriod;
    
    int lastEstimatedPeriod;
    bool lastFrameWasPitched;
    
    double samplerate;
    
    SampleType confidenceThresh;  // if the lowest asdf data value is above this thresh, the frame of audio is determined to be unpitched
    
    AudioBuffer asdfBuffer; // calculated ASDF values will be placed in this buffer
    
    int chooseIdealPeriodCandidate (const SampleType* asdfData, const int asdfDataSize, const int minIndex);
    
    int samplesToFirstZeroCrossing (const SampleType* inputAudio, const int numSamples);
    
    void getNextBestPeriodCandidate (juce::Array<int>& candidates, const SampleType* asdfData, const int dataSize);
    
    
    juce::Array<int> periodCandidates;
    juce::Array<float> candidateDeltas;
    juce::Array<SampleType> weightedCandidateConfidence;
    
    
    // for effenciency, I'm storing some common constexprs that are used in several functions as private members... ¯\_(ツ)_/¯
    static constexpr SampleType zero = SampleType(0.0);
    static constexpr SampleType pnt5 = SampleType(0.5);
    static constexpr SampleType one = SampleType(1.0);
    static constexpr SampleType two = SampleType(2.0);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

    
} // namespace



