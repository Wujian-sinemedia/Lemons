#pragma once

namespace bav::dsp
{
template < typename SampleType, class StateType, template < typename NumericType > class EngineType,
           std::enable_if_t< std::is_base_of< PluginState, StateType >::value >*                         = nullptr,
           std::enable_if_t< std::is_base_of< Engine< SampleType >, EngineType< SampleType > >::value >* = nullptr >
class StatefulEngine : public EngineType< SampleType >
{
public:
    StatefulEngine (StateType& stateToUse)
        : EngineType< SampleType > (stateToUse) { }
};


template < class StateType, template < typename SampleType > class EngineType,
           std::enable_if_t< std::is_base_of< PluginState, StateType >::value >*               = nullptr,
           std::enable_if_t< std::is_base_of< Engine< float >, EngineType< float > >::value >* = nullptr >
class Processor : public ProcessorBase
{
public:
    Processor (juce::AudioProcessor::BusesProperties busesLayout)
        : ProcessorBase (state, floatEngine, doubleEngine, busesLayout)
    {
        state.addTo (*this);
    }

    StateType& getState() { return state; }

private:
    StateType state;

    StatefulEngine< float, StateType, EngineType >  floatEngine {state};
    StatefulEngine< double, StateType, EngineType > doubleEngine {state};
};


template < class ProcessorType, class ComponentType, int width = 450, int height = 300, std::enable_if_t< std::is_base_of< ProcessorBase, ProcessorType >::value >* = nullptr >
struct ProcessorWithEditor : ProcessorType
{
    bool hasEditor() const final { return true; }

    juce::AudioProcessorEditor* createEditor() final
    {
        return new gui::PluginEditor< ComponentType > (*this, {width, height}, this->getState());
    }
};

}  // namespace bav::dsp
