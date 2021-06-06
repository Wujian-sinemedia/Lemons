
#pragma once

namespace bav
{

class ParameterProcessorBase
{
public:
    ParameterProcessorBase (ParameterList& listToUse);
    virtual ~ParameterProcessorBase() = default;
    
    template<typename SampleType>
    void process (juce::AudioBuffer<SampleType>& audio, juce::MidiBuffer& midi);
    
private:
    virtual void renderChunk (juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi) = 0;
    virtual void renderChunk (juce::AudioBuffer<double>& audio, juce::MidiBuffer& midi) = 0;
    
    template<typename SampleType>
    void processInternal (juce::AudioBuffer<SampleType>& audio, juce::MidiBuffer& midi,
                          int startSample, int numSamples);
    
    ParameterList& list;
    juce::MidiBuffer midiStorage;
};

}
