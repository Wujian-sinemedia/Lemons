#pragma once


namespace lemons::gui::components
{

using namespace plugin;

class PluginDetailView : public Component
{
public:
    
    explicit PluginDetailView (const juce::PluginDescription& descriptionToUse);
    
    [[nodiscard]] std::unique_ptr<juce::AudioPluginInstance> createPluginInstance (double initialSampleRate = 44100.,
                                                                                   int initialBlocksize = 512) const;
    
    juce::PluginDescription description;
    
private:
    void resized() final;
    
    Label name { description.name };
    Label descriptiveName { description.descriptiveName };
    Label version { description.version };
    
    // is instrument
    // has shared container
    
    Label numInChannels { "Num input channels: " + String(description.numInputChannels) };
    Label numOutChannels { "Num output channels: " + String(description.numOutputChannels) };
    
    Label category { "Category: " + description.category };
    Label manufacturer { "Manufacturer: " + description.manufacturerName };
    
    // format name
    // file or ID
    // last file mod time
    // last info update
};


/*-------------------------------------------------------------------------------------------------------------------------*/


class PluginCategoryComponent : public Component
{
public:
    
    explicit PluginCategoryComponent (const PluginCategory& category);
    
private:
    juce::OwnedArray<PluginDetailView> plugins;
    juce::OwnedArray<PluginCategoryComponent> subcategories;
};

}
