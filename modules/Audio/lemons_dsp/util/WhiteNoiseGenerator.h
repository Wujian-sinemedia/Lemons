#pragma once

namespace lemons::dsp
{

template<typename SampleType>
class WhiteNoiseGenerator
{
public:
    SampleType getNextSample();
    
    void getSamples (SampleType* output, int numSamples);
    
    void getSamples (AudioBuffer<SampleType>& output, int channel = 0);
    
private:
    juce::Random rand;
};

}
