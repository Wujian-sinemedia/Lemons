
#pragma once

#include "GrainExtractor/GrainExtractor.h"
#include "AnalysisGrain/AnalysisGrain.h"


namespace bav
{
template < typename SampleType >
class PsolaAnalyzer
{
    using Analysis_Grain = AnalysisGrain< SampleType >;
    
    
public:
    PsolaAnalyzer();
    
    void initialize();
    
    void prepare (int blocksize, double sr);
    
    void reset();
    
    void clearUnusedGrains();
    
    void releaseResources();
    
    void analyzeInput (const SampleType* inputSamples, const int numSamples);
    
    Analysis_Grain* findClosestGrain (int idealBufferPos) const;
    
    Analysis_Grain* findBestNewGrain (Analysis_Grain* prevGrain) const;
    
    int getLatency() const;
    

private:
    double samplerate = 0;
    
    Analysis_Grain* getEmptyGrain() const;
    
    PitchDetector< SampleType >  pitchDetector;
    
    GrainExtractor< SampleType > grainExtractor;
    juce::Array< int >           indicesOfGrainOnsets;
    
    juce::OwnedArray< Analysis_Grain > analysisGrains;
    
    static constexpr auto numAnalysisGrains = 48;
};


} // namespace bav
