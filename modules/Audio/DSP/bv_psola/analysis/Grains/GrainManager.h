#pragma once

#include "GrainExtractor/GrainExtractor.h"
#include "GrainStorage/GrainStorage.h"

namespace bav::dsp::psola
{
template < typename SampleType >
class AnalysisGrainManager
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    AnalysisGrainManager()  = default;
    ~AnalysisGrainManager() = default;

    void analyzeInput (const SampleType* inputSamples,
                       int               numSamples,
                       int               period);

    void prepare (int blocksize);

    int getStartOfClosestGrain (int sampleIndex) const;

    const CircularBuffer< SampleType >& getBuffer() const;

private:
    juce::Array< int > grainIndices;  // the sample indices of the grain onsets, starting from sample 0 of the current frame

    AnalysisGrainExtractor< SampleType > extractor;
    AnalysisGrainStorage< SampleType >   storage;
};

}  // namespace bav::dsp::psola
