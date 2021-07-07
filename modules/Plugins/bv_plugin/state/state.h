
#pragma once

namespace bav
{

struct PluginDimensions : SerializableData
{
    using SerializableData::SerializableData;
    
    void serialize (TreeReflector& ref) final;
    
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
    
protected:
    void serialize (TreeReflector& ref) override;
    
private:
    ParameterList& list;
};

}  // namespace bav
