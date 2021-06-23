
#pragma once


namespace bav
{
template < typename SampleType >
class GrainExtractor
{
    using IArray = juce::Array< int >;
    using FArray = juce::Array< float >;

public:
    GrainExtractor()  = default;
    ~GrainExtractor() = default;

    void prepare (const int maxBlocksize);

    void releaseResources();

    void getGrainOnsetIndices (IArray&           targetArray,
                               const SampleType* inputSamples,
                               const int         numSamples,
                               const int         period);


private:
    void findPsolaPeaks (IArray&           targetArray,
                         const SampleType* reading,
                         const int         totalNumSamples,
                         const int         period);

    int findNextPeak (const int         frameStart,
                      const int         frameEnd,
                      int               predictedPeak,
                      const SampleType* reading,
                      const IArray&     targetArray,
                      const int         period,
                      const int         grainSize);

    void sortSampleIndicesForPeakSearching (IArray&   output,
                                            const int startSample,
                                            const int endSample,
                                            const int predictedPeak);

    void getPeakCandidateInRange (IArray&           candidates,
                                  const SampleType* input,
                                  const int         startSample,
                                  const int         endSample,
                                  const int         predictedPeak,
                                  const IArray&     searchingOrder);

    int chooseIdealPeakCandidate (const IArray&     candidates,
                                  const SampleType* reading,
                                  const int         deltaTarget1,
                                  const int         deltaTarget2);

    int choosePeakWithGreatestPower (const IArray&     candidates,
                                     const SampleType* reading);

    IArray peakIndices;
    IArray peakCandidates;
    IArray peakSearchingOrder;

    FArray candidateDeltas;
    IArray finalHandful;
    FArray finalHandfulDeltas;

    static constexpr auto numPeaksToTest          = 10;
    static constexpr auto defaultFinalHandfulSize = 5;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrainExtractor)
};


}  // namespace bav
