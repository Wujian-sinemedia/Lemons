
#pragma once

namespace bav
{

struct PluginDimensions : SerializableData
{
    using SerializableData::SerializableData;
    
    void toValueTree (ValueTree& tree);
    void fromValueTree (const ValueTree& tree);
    
    int width, height;
};

class PluginState : public SerializableData
{
public:
    PluginState (ParameterList& listToUse, String pluginName = "");
    
    ParameterList& getParameters();
    
    void setDimensions (int width, int height);
    
    virtual void addTo (juce::AudioProcessor& processor);
    
    
    ToggleParam mainBypass {"Main bypass", "Main bypass", false};
    
    PluginDimensions dimensions;
    
private:
    void toValueTree (ValueTree&) override { }
    void fromValueTree (const ValueTree&) override { }
    
    ParameterList& list;
};

}  // namespace bav
