
#pragma once

namespace bav
{
class PluginState : public SerializableData
{
public:
    PluginState (ParameterList& listToUse, String name = "State");
    
    ParameterList& getParameters();
    
    virtual void addTo (juce::AudioProcessor& processor);
    
    
    ToggleParam mainBypass {"Main bypass", "Main bypass", false};
    
private:
    void toValueTree (ValueTree&) override { }
    void fromValueTree (const ValueTree&) override { }
    
    ParameterList& list;
};

}  // namespace bav
