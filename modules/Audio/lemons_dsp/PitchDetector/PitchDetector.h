
/*
    ASDF-based pitch detector
*/

#pragma once

namespace lemons::dsp
{
template < typename SampleType >
class PitchDetector
{
public:
    PitchDetector (int minFreqHz = 60, int maxFreqHz = 2500);
    ~PitchDetector() = default;

    void releaseResources();

    float detectPitch (const AudioBuffer< SampleType >& inputAudio);
    float detectPitch (const SampleType* inputAudio, int numSamples);

    void setConfidenceThresh (SampleType newThresh);
    void setSamplerate (double newSamplerate);

    int getLatencySamples() const noexcept;

    juce::Range< int > getCurrentLegalPeriodRange() const;

private:
    int chooseIdealPeriodCandidate (const SampleType* asdfData,
                                    int               asdfDataSize,
                                    int               minIndex);

    const int minHz, maxHz;
    int       minPeriod {0}, maxPeriod {0};

    int  lastEstimatedPeriod {0};
    bool lastFrameWasPitched {false};

    double samplerate {0.0};

    // if the highest periodicity value is below this thresh, the frame of audio is determined to be unpitched
    // 1. = only perfect sine waves are pitched
    // 0. = nothing is unpitched
    SampleType confidenceThresh {static_cast< SampleType > (0.15)};

    AudioBuffer< SampleType > asdfBuffer {1, 512};  // calculated ASDF values will be placed in this buffer

    Array< int >        periodCandidates;
    Array< int >        candidateDeltas;
    Array< SampleType > weightedCandidateConfidence;

    AudioBuffer< SampleType >     filteringBuffer {1, 512};
    filters::Filter< SampleType > loCut, hiCut;

    static constexpr int numPeriodCandidatesToTest = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

}  // namespace bav::dsp
