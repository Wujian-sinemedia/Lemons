#pragma once

/*
 This class encapsulates all the functionality necessary to draw a component and send it to a connected Push display.
 */

namespace ableton
{

class PushDisplay  :    public PushDisplayBridge
{
public:
    PushDisplay()
        : PushDisplayBridge (interface)
    { }
    
    int getWidth() const { return getDisplayWidth(); }
    int getHeight() const { return getDisplayHeight(); }
    juce::Rectangle<int> getSize() const { return getDisplaySize(); }
    
protected:
    PushDisplayInterface interface;
};

}  // namespace
