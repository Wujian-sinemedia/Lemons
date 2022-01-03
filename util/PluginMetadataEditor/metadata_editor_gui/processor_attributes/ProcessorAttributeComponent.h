
#pragma once

namespace lemons::PluginMetadataEditor::gui
{

class ProcessorAttributeComponent final : public juce::Component
{
public:
    
    explicit ProcessorAttributeComponent (plugin::ProcessorAttributes& attributesToUse, std::function<void()> callback);
    
private:
    
    std::function<void()> stateChanged;
    
    plugin::ProcessorAttributes& attributes;
    
    BusesLayoutComponent busLayoutView { attributes, [this](){ this->stateChanged(); } };
    
    ToggleTextButton acceptsMidi { TRANS ("Accepts MIDI"), attributes.acceptsMidi, [this](bool v){ this->attributes.acceptsMidi = v; this->stateChanged(); } };
    
    ToggleTextButton producesMidi { TRANS ("Produces MIDI"), attributes.producesMidi, [this](bool v){ this->attributes.producesMidi = v; this->stateChanged(); } };
    
    ToggleTextButton supportsMPE { TRANS ("Supports MPE"), attributes.supportsMPE, [this](bool v){ this->attributes.supportsMPE = v; this->stateChanged(); } };
    
    ToggleTextButton isMidiEffect { TRANS ("Is MIDI effect"), attributes.isMidiEffect, [this](bool v){ this->attributes.isMidiEffect = v; this->stateChanged(); } };
    
    TextEntry name { TRANS (attributes.name), [this](const String& v){ this->attributes.name = v; this->stateChanged(); } };
    
    TextEntry alternateNames { attributes.alternateNames.joinIntoString (";"), [this](const String& v)
        {
            this->attributes.alternateNames = juce::StringArray::fromTokens (v, ";, \n", "\"'");
            this->stateChanged();
        } };
};

}
