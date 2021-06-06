#pragma once

namespace bav::gui
{

struct DarkModeSentinel : private juce::Timer,
                          private BoolParameter::Listener
{
    DarkModeSentinel (BoolParameter& paramToUse, juce::Component& componentToUse)
    : BoolParameter::Listener (paramToUse),
    darkModeParameter (paramToUse),
    componentToRepaint (componentToUse)
    {
#if JUCE_MAC
        Timer::startTimerHz (10);
#endif
    }
    
    virtual ~DarkModeSentinel() override
    {
#if JUCE_MAC
        Timer::stopTimer();
#endif
    }
    
private:
    void timerCallback() final
    {
#if JUCE_MAC
        darkModeParameter.set (juce::Desktop::isOSXDarkModeActive());
#endif
    }
    
    void paramValueChanged (bool) final
    {
        componentToRepaint.repaint();
    }
    
    BoolParameter&   darkModeParameter;
    juce::Component& componentToRepaint;
};

}  // namespace
