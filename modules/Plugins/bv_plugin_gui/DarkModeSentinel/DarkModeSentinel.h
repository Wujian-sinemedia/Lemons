
#pragma once

namespace bav::gui
{

struct DarkModeSentinel : private BoolParameter::Listener
{
    DarkModeSentinel (BoolParameter& paramToUse, juce::Component& componentToUse);
    
private:
    void paramValueChanged (bool) final;
    
    BoolParameter&   darkModeParameter;
    juce::Component& componentToRepaint;
    
#if JUCE_MAC
    events::TimerCallback t {[&]
        { darkModeParameter.set (juce::Desktop::isOSXDarkModeActive()); }};
#endif
};

}
