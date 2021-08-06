#pragma once

namespace bav::plugin
{
template < class StateType, template < typename SampleType > class EngineType, BV_MUST_INHERIT_FROM (StateType, StateBase), BV_MUST_INHERIT_FROM (EngineType< float >, dsp::Engine< float >) >
class Processor : public ProcessorBase
{
public:
    Processor (juce::AudioProcessor::BusesProperties busesLayout)
        : ProcessorBase (state, floatEngine, doubleEngine, busesLayout)
    {
        state.addTo (*this);
        state.getParameters().setUndoManager (undo);
        toggler.setUndoManager (undo);
    }

protected:
    StateType    state;
    StateToggler toggler {state};
    UndoManager  undo {state};

private:
    void getStateInformation (juce::MemoryBlock& block) final
    {
        serializing::toBinary (serializer, block);
    }

    void setStateInformation (const void* data, int size) final
    {
        serializing::fromBinary (data, size, serializer);
    }

    EngineType< float >  floatEngine {state};
    EngineType< double > doubleEngine {state};

    StateSerializer serializer {state, toggler};
};


template < class ProcessorType, class ComponentType, BV_MUST_INHERIT_FROM (ProcessorType, ProcessorBase) >
struct ProcessorWithEditor : ProcessorType
{
    ProcessorWithEditor (int width = 450, int height = 300)
        : w (width), h (height)
    {
        jassert (w > 0 && h > 0);
        this->state.dimensions = {w, h};
    }

    bool hasEditor() const final { return true; }

    juce::AudioProcessorEditor* createEditor() final
    {
        return new PluginEditor< ComponentType > (*this, this->state, this->toggler, this->undo);
    }

private:
    int w, h;
};

}  // namespace bav::plugin
