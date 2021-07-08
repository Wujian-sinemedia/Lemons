
#pragma once

namespace bav::gui
{

struct DarkModeSentinel : private plugin::BoolParameter::Listener
{
    DarkModeSentinel (plugin::BoolParameter& paramToUse, juce::Component& componentToUse);
    
private:
    void paramValueChanged (bool) final;
    
    plugin::BoolParameter&   darkModeParameter;
    juce::Component& componentToRepaint;
    
#if JUCE_MAC
    events::TimerCallback t {[&]
        { darkModeParameter.set (juce::Desktop::isOSXDarkModeActive()); }};
#endif
};

}
