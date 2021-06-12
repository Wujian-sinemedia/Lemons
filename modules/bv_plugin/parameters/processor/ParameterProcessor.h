
#pragma once

namespace bav
{

template<typename SampleType>
class ParameterProcessorBase : public dsp::MidiChoppingProcessor<SampleType>
{
public:
    ParameterProcessorBase (ParameterList& listToUse);
    virtual ~ParameterProcessorBase() = default;
    
private:
    void handleMidiMessage (const juce::MidiMessage& m) final;
    
    ParameterList& list;
};

}
