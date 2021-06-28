#pragma once

namespace bav::dsp::osc::supersaw
{

template<typename SampleType>
class Node
{
public:
    void resetPhase();
    
    void setFrequency (SampleType frequency, SampleType samplerate);
    
    SampleType getSample();
    
private:
    Saw<SampleType> saw;
};

}
