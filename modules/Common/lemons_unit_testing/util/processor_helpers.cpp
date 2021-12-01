namespace lemons::tests
{

bool allParameterNamesAreUnique (juce::AudioProcessor& processor)
{
    juce::StringArray names;
    
    for (const auto* parameter : processor.getParameters())
    {
        const auto name = parameter->getName(50);
        
        if (names.contains (name))
            return false;
        
        names.add (name);
    }
    
    return true;
}

juce::AudioProcessorParameter* getNamedParameter (juce::AudioProcessor& processor,
                                                  const String& name)
{
    for (auto* parameter : processor.getParameters())
        if (parameter->getName(50) == name)
            return parameter;
    
    return nullptr;
}

ProcessorParameterState getStateOfProcessorParameters (juce::AudioProcessor& processor)
{
    ProcessorParameterState state;
    
    for (const auto* parameter : processor.getParameters())
    {
        ProcessorParameterData data;
        data.name = parameter->getName(50);
        data.value = parameter->getValue();
        state.add (data);
    }
    
    return state;
}

bool processorMatchesParameterState (juce::AudioProcessor& processor, const ProcessorParameterState& state)
{
    for (const auto& data : state)
        if (getNamedParameter (processor, data.name)->getValue() != data.value)
            return false;
    
    return true;
}

}
