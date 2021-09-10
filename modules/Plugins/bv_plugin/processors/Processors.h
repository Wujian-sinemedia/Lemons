#pragma once

namespace bav::plugin
{
template < class StateType, template < typename SampleType > class EngineType, BV_MUST_INHERIT_FROM (StateType, StateBase), BV_MUST_INHERIT_FROM (EngineType< float >, dsp::Engine< float >) >
class Processor : public ProcessorBase
{
public:
    Processor (juce::AudioProcessor::BusesProperties busesLayout)
        : ProcessorBase (getState(), floatEngine, doubleEngine, busesLayout)
    {
        stateData.addTo (*this);
    }

protected:
    StateType& getState() { return state.state; }

    PluginState< StateType > state;

    using State_Type_t = StateType;

private:
    void getStateInformation (juce::MemoryBlock& block) final
    {
        serializing::toBinary (state, block);
    }

    void setStateInformation (const void* data, int size) final
    {
        serializing::fromBinary (data, size, state);
    }

    StateType& stateData {getState()};

    EngineType< float >  floatEngine {stateData};
    EngineType< double > doubleEngine {stateData};
};


template < template < typename SampleType > class EngineType >
using StatelessProcessor = Processor< StateBase, EngineType >;


class GUIBase;

template < class ProcessorType, class ComponentType, BV_MUST_INHERIT_FROM (ComponentType, GUIBase) >
struct ProcessorWithEditor : ProcessorType
{
    ProcessorWithEditor (int width = 450, int height = 300)
        : w (width), h (height)
    {
        jassert (isValidGuiSize (w, h));

        auto& size = this->getState().dimensions;

        if (! isValidGuiSize (size.w(), size.h()))
            size = {w, h};
    }

    bool hasEditor() const final { return true; }

    juce::AudioProcessorEditor* createEditor() final
    {
        return new PluginEditor< ComponentType, typename ProcessorType::State_Type_t > (*this, this->state);
    }

private:
    constexpr bool isValidGuiSize (int width, int height)
    {
        return width > 0 && height > 0;
    }

    int w, h;
};

}  // namespace bav::plugin
