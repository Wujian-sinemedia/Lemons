#pragma once

#include "Node.h"

namespace bav::dsp::osc
{

template<typename SampleType>
class SuperSaw : public Oscillator<SampleType>
{
public:
    using Node = supersaw::Node<SampleType>;
    
    SuperSaw();
    
    void setDetuneAmount (int totalPitchSpreadInCents);
    int  getPitchSpreadCents() const;
    
private:
    SampleType getSample() final;
    void setFrequency (SampleType frequency, SampleType sampleRate) final;
    void resetPhase() final;
    
    juce::OwnedArray<Node> saws;
    
    int totalSpreadCents {0};
    
    SampleType lastFrequency {440.}, samplerate {44100.};
};

}
