
#pragma once


namespace bav::dsp
{
template < typename SampleType >
class PsolaShifter
{
    using Analyzer = PsolaAnalyzer< SampleType >;

public:
    PsolaShifter (Analyzer& parentAnalyzer);
    
    void setPitch (float desiredFrequency, double samplerate);
    
    void       getSamples (SampleType* outputSamples, int numSamples);
    SampleType getNextSample();
    
private:
    Analyzer& analyzer;
    
    int desiredPeriod {0};
};


}  // namespace bav::dsp
