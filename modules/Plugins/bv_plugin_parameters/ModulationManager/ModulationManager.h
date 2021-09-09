#pragma once

#include <bv_oscillators/bv_oscillators.h>

namespace bav::plugin
{

template<typename SampleType>
class ModulationManager : public SerializableData
{
public:
    
    
private:
    /*------------------------------*/
    struct LFO : dsp::osc::ChoosableOscillator<SampleType>
    {
        void prepareToPlay (int numSamples, double samplerate);
        
        void processNextBlock (int numSamples);
        
        SampleType getAndDontAdvance() const;
        SampleType getAndAdvance();
        
    private:
        int currentTick {0};
        
        AudioBuffer<SampleType> storage;
    };
    /*------------------------------*/
    
    void serialize (TreeReflector& ref);
    
    OwnedArray<LFO> lfos;
};

}
