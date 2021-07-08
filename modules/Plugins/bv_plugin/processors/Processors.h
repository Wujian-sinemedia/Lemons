#pragma once

namespace bav::plugin
{
template < class StateType, template < typename SampleType > class EngineType,
           std::enable_if_t< std::is_base_of< State, StateType >::value >*               = nullptr,
           std::enable_if_t< std::is_base_of< dsp::Engine< float >, EngineType< float > >::value >* = nullptr >
class Processor : public ProcessorBase
{
public:
    Processor (juce::AudioProcessor::BusesProperties busesLayout)
        : ProcessorBase (state, floatEngine, doubleEngine, busesLayout)
    {
        state.addTo (*this);
    }

protected:
    StateType state;

private:
    EngineType< float >  floatEngine {state};
    EngineType< double > doubleEngine {state};
};


template < class ProcessorType, class ComponentType,
           std::enable_if_t< std::is_base_of< ProcessorBase, ProcessorType >::value >* = nullptr >
struct ProcessorWithEditor : ProcessorType
{
    ProcessorWithEditor (int width = 450, int height = 300)
        : w (width), h (height)
    {
    }

    bool hasEditor() const final { return true; }

    juce::AudioProcessorEditor* createEditor() final
    {
        return new PluginEditor< ComponentType > (*this, {w, h}, this->state);
    }

private:
    int w, h;
};

}  // namespace bav::dsp
