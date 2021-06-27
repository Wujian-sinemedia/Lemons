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
    
    const AnalysisGrainStorage< SampleType >& getStorage() const;

private:
    AnalysisGrainExtractor< SampleType > extractor;
    AnalysisGrainStorage< SampleType >   storage;
};

}  // namespace bav::dsp::psola
