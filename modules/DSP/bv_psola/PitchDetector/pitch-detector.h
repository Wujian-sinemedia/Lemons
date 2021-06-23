
/*
    ASDF-based pitch detector
*/

#pragma once

#include <bv_core/bv_core.h>

namespace bav::dsp
{
template < typename SampleType >
class PitchDetector
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;


public:
    PitchDetector();

    ~PitchDetector() = default;

    void initialize();

    void releaseResources();

    float detectPitch (const AudioBuffer& inputAudio);

    float detectPitch (const SampleType* inputAudio, const int numSamples);


    int getLatencySamples() const noexcept { return 2 * maxPeriod; }

    void setHzRange (const int newMinHz, const int newMaxHz);

    void setConfidenceThresh (const SampleType newThresh)
    {
        confidenceThresh = newThresh;
    }

    void setSamplerate (const double newSamplerate);

    juce::Range< int > getCurrentLegalPeriodRange() const
    {
        return {minPeriod, maxPeriod};
    }

    /*
    */

private:
    int chooseIdealPeriodCandidate (const SampleType* asdfData,
                                    const int         asdfDataSize,
                                    const int         minIndex);

    void getNextBestPeriodCandidate (juce::Array< int >& candidates,
                                     const SampleType*   asdfData,
                                     const int           dataSize);

    int samplesToFirstZeroCrossing (const SampleType* inputAudio,
                                    const int         numSamples);

    /*
    */

    int minHz, maxHz;
    int minPeriod, maxPeriod;

    int  lastEstimatedPeriod;
    bool lastFrameWasPitched;

    double samplerate;

    SampleType
        confidenceThresh;  // if the lowest asdf data value is above this thresh, the frame of audio is determined to be unpitched

    AudioBuffer asdfBuffer;  // calculated ASDF values will be placed in this buffer

    juce::Array< int >        periodCandidates;
    juce::Array< int >        candidateDeltas;
    juce::Array< SampleType > weightedCandidateConfidence;

    static constexpr int numPeriodCandidatesToTest = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

}  // namespace bav::dsp
