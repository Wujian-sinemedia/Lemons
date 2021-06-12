#pragma once

namespace bav::dsp
{

template<typename SampleType>
class MidiChoppingProcessor
{
public:
    MidiChoppingProcessor();
    virtual ~MidiChoppingProcessor() = default;
    
    void process (juce::AudioBuffer<SampleType>& audio, juce::MidiBuffer& midi);
    
private:
    void processInternal (juce::AudioBuffer<SampleType>& audio, juce::MidiBuffer& midi,
                          int startSample, int numSamples);
    
    virtual void handleMidiMessage (const juce::MidiMessage& m) = 0;
    virtual void renderChunk (juce::AudioBuffer<SampleType>& audio, juce::MidiBuffer& midi) = 0;
    
    juce::MidiBuffer midiStorage;
};

}
