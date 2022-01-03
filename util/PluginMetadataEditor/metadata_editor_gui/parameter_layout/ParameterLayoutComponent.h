#pragma once

#pragma once

namespace lemons::PluginMetadataEditor::gui
{

class ParameterMetadataComponent final : public juce::Component
{
public:
    explicit ParameterMetadataComponent (plugin::ParameterTraits& traitsToUse, std::function<void()> callback);
    
    [[nodiscard]] plugin::ParameterTraits& getTraits() const { return traits; }
    
private:
    std::function<void()> stateChanged;
    
    plugin::ParameterTraits& traits;
    
    ToggleTextButton isAutomatable { TRANS ("Is automatable"), traits.isAutomatable, [this](bool v){ this->traits.isAutomatable = v; this->stateChanged(); } };
    
    ToggleTextButton isMetaParameter { TRANS ("Is automatable"), traits.isMetaParameter, [this](bool v){ this->traits.isMetaParameter = v; this->stateChanged(); } };
    
    TextEntry name { TRANS (traits.name), [this](const String& v){ this->traits.name = v; this->stateChanged(); } };
    
    TextEntry label { TRANS (traits.label), [this](const String& v){ this->traits.label = v; this->stateChanged(); } };
    
    /*
     value type
     minimum
     maximum
     initial default
     range
     category
     */
};


class ParameterLayoutComponent final : public juce::Component
{
public:
    explicit ParameterLayoutComponent (plugin::ParameterLayout& layoutToUse, std::function<void()> callback);
    
private:
    ParameterMetadataComponent& addNewParameter();
    
    void removeParameter (plugin::ParameterTraits& traits);
    
    std::function<void()> stateChanged;
    
    plugin::ParameterLayout& layout;
    
    juce::OwnedArray<ParameterMetadataComponent> parameterComponents;
};

}
