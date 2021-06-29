
#pragma once

namespace bav
{
class PluginState : public SerializableData
{
public:
    using SerializableData::SerializableData;
    
    virtual ParameterList& getParameters() = 0;
    
    virtual void addTo (juce::AudioProcessor& processor)
    {
        getParameters().addParametersTo (processor);
    }
    
private:
    void toValueTree (ValueTree&) override { }
    void fromValueTree (const ValueTree&) override { }
};

}  // namespace bav
