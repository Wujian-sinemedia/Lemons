#pragma once

/*
 This is a component class that will automatically be painted onto any connected Ableton Push display. You can also call repaint() as you normally would, to use this component in an actual app or plugin as well.
 */

namespace ableton
{

class PushComponent :   public  juce::Component,
                        private juce::Timer
{
public:
    PushComponent (int framerate = 30)
    {
        // automatically resize to the size of the Push display
        setSize (pushDisplay.getWidth(),
                 pushDisplay.getHeight());
        
        startTimerHz (framerate);
    }
    
    virtual ~PushComponent() override
    {
        stopTimer();
    }
    
protected:
    PushDisplay pushDisplay;
   
private:
    void timerCallback() override final
    {
        pushDisplay.drawComponent (*this);
    }
};

}  // namespace
