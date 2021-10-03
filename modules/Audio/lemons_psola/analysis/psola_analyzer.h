#pragma once

#include <lemons_dsp/lemons_dsp.h>
#include "GrainStorage/GrainStorage.h"


namespace lemons::dsp::psola
{
template < typename SampleType >
class Shifter;

/** Performs analysis of the incoming signal, to prepare for the Shifter's resynthesis.
    This detects the pitch of the input signal and splits the incoming audio frame into analysis grains, ready to be respaced by Shifter objects. \n
    This is meant to be used with perfectly monophonic signals.
    @see Shifter, dsp::PitchDetector
 */
template < typename SampleType >
class Analyzer
{
public:
    using Storage = AnalysisGrainStorage< SampleType >;

    /** Prepares the analyzer. */
    void prepare (double sampleRate, int blocksize);

    /** Reports the latency in samples of the PSOLA algorithm.
        This is determined by the analyzer's internal PitchDetector.
        @see PitchDetector::getLatencySamples()
     */
    int getLatencySamples() const;

    /** Analyzes a stream of input samples.
        This object can only be used with one channel of audio at a time.
     */
    void analyzeInput (const AudioBuffer< SampleType >& input, int channel = 0);

    /** Analyzes a stream of input samples. */
    void analyzeInput (const SampleType* samples, int numSamples);

    /** Returns the current analysis grain length, in samples. */
    int getGrainLength() const;

    /** Returns the current period of the input signal, in samples. */
    int getPeriod() const;

    /** Returns the current detected fundamental frequency of the input audio. */
    float getFrequency() const;

private:
    /*--------------------------------------------------------*/

    class PeakFinder
    {
    public:
        using IArray = juce::Array< int >;
        using FArray = juce::Array< float >;

        void findPeaks (IArray&           targetArray,
                        const SampleType* reading,
                        int               totalNumSamples,
                        int               period);

        void releaseResources();

        void prepare (int blocksize);

    private:
        int findNextPeak (int               frameStart,
                          int               frameEnd,
                          int               predictedPeak,
                          const SampleType* reading,
                          const IArray&     targetArray,
                          int               period,
                          int               grainSize);

        int chooseIdealPeakCandidate (const IArray&     candidates,
                                      const SampleType* reading,
                                      int               deltaTarget1,
                                      int               deltaTarget2);

        void getPeakCandidateInRange (
            IArray&           candidates,
            const SampleType* input,
            int               startSample,
            int               endSample,
            int               predictedPeak,
            const IArray&     searchingOrder);

        int choosePeakWithGreatestPower (const IArray& candidates, const SampleType* reading);


        IArray peakIndices;
        IArray peakCandidates;
        IArray peakSearchingOrder;

        FArray candidateDeltas;
        IArray finalHandful;
        FArray finalHandfulDeltas;

        static constexpr auto numPeaksToTest          = 10;
        static constexpr auto defaultFinalHandfulSize = 5;
    };

    /*--------------------------------------------------------*/

    class GrainExtractor
    {
    public:
        void prepare (int blocksize);

        void releaseResources();

        void analyzeInput (const SampleType* inputSamples,
                           int               numSamples,
                           int               period);

        const juce::Array< int >& getIndices() const;

    private:
        PeakFinder peakFinder;

        juce::Array< int > grainStartIndices;
    };

    /*--------------------------------------------------------*/

    friend class Shifter< SampleType >;

    int          getNextUnpitchedPeriod();
    juce::Random rand;

    double samplerate {0.};
    int    currentPeriod {0};

    PitchDetector< SampleType > pitchDetector;
    GrainExtractor              grainExtractor;
    Storage                     grainStorage;

    events::Broadcaster broadcaster;
};


}  // namespace lemons::dsp::psola
