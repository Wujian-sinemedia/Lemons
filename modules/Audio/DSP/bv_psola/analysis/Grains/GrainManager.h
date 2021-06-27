#pragma once

#include "GrainExtractor/GrainExtractor.h"

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

private:
    AnalysisGrainExtractor< SampleType > extractor;
};

}  // namespace bav::dsp::psola
