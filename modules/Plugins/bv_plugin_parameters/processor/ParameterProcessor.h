
#pragma once

namespace bav::plugin
{
template < typename SampleType >
class ParameterProcessorBase : public dsp::MidiChoppingProcessor< SampleType >
{
public:
    ParameterProcessorBase (ParameterList& listToUse);
    virtual ~ParameterProcessorBase() = default;

private:
    void handleMidiMessage (const MidiMessage& m) final;

    virtual void midiMessageCallback (const MidiMessage&) { }

    ParameterList& list;
};

}  // namespace bav::plugin
