
#pragma once

#include "SynthesisGrain/SynthesisGrain.h"


namespace bav::dsp
{
template < typename SampleType >
class PsolaShifter
{
    using Analyzer        = PsolaAnalyzer< SampleType >;
    using Synthesis_Grain = SynthesisGrain< SampleType >;
    
public:
    PsolaShifter (Analyzer& parentAnalyzer);
    
    void prepare();
    
    void newBlockComing (int prevBlocksize) noexcept;
    
    void reset();
    
    void releaseResources();
    
    void bypassedBlockRecieved (int numSamples);
    
    void getSamples (SampleType* outputSamples, const int numSamples, const int newPeriod);
    
    SampleType getNextSample (const int newPeriod, const int origPeriod);
    

private:
    void startNewGrain (const int newPeriod, const int origPeriod, AnalysisGrain< SampleType >* lastGrain);
    
    Synthesis_Grain* getAvailableGrain();
    
    bool anyGrainsAreActive() const;
    
    
    Analyzer& analyzer;
    
    juce::OwnedArray< Synthesis_Grain > synthesisGrains;
    
    int nextSynthesisPitchMark = 0;
    
    int nextAnalysisPitchMark = 0;
    
    static constexpr auto numSynthesisGrains = 48;
};


} // namespace bav
