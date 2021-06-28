#pragma once

namespace bav::dsp::osc
{

template<typename SampleType>
class SuperSaw : public Oscillator<SampleType>
{
public:
    SuperSaw();
    
    void setDetuneAmount (int totalPitchSpreadInCents);
    int  getPitchSpreadCents() const;
    
private:
    SampleType getSample() final;
    void setFrequency (SampleType frequency, SampleType sampleRate) final;
    void resetPhase() final;
    
    juce::OwnedArray<Saw<SampleType>> saws;
    
    int totalSpreadCents {0};
    
    SampleType lastFrequency {440.}, samplerate {44100.};
};

}
