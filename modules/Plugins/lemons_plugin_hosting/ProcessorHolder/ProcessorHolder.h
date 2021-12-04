#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <lemons_core/lemons_core.h>


namespace lemons::plugin
{

using juce::String;


struct ProcessorHolder
{
    explicit ProcessorHolder (juce::AudioProcessor& processorToUse);
    
    virtual ~ProcessorHolder() = default;
    
    juce::AudioProcessor* operator->();
    const juce::AudioProcessor* operator->() const;
    
    template<typename SampleType>
    void prepareForPlayback (double samplerate, int blocksize);
    
    [[nodiscard]] juce::AudioProcessorParameter* getNamedParameter (const String& name);
    
    [[nodiscard]] const juce::AudioProcessorParameter* getNamedParameter (const String& name) const;
    
    template<typename ParameterType>
    [[nodiscard]] ParameterType* getTypedParameter (const String& name)
    {
        if (auto* p = getNamedParameter (name))
            return dynamic_cast<ParameterType*>(p);
        
        return nullptr;
    }
    
    template<typename ParameterType>
    [[nodiscard]] const ParameterType* getTypedParameter (const String& name) const
    {
        if (const auto* p = getNamedParameter (name))
            return dynamic_cast<const ParameterType*>(p);
        
        return nullptr;
    }
    
    struct ParameterStateData
    {
        String name;
        float value;
    };
    
    using ParameterState = juce::Array<ParameterStateData>;
    
    [[nodiscard]] ParameterState getStateOfParameters() const;
    
    void setStateOfParameters (const ParameterState& state);
    
    
    juce::AudioProcessor& processor;
};


template<typename ProcessorType, LEMONS_MUST_INHERIT_FROM(ProcessorType, juce::AudioProcessor)>
struct TypedProcessorHolder final : public ProcessorHolder
{
    TypedProcessorHolder(): ProcessorHolder(typedProcessor) { }
    
    ProcessorType typedProcessor;
};

}
