#pragma once

#include <bv_serializing/bv_serializing.h>
#include <bv_mobile_utils/bv_mobile_utils.h>

namespace bav
{
struct DefaultWindowBase : public juce::DocumentWindow
{
public:
    DefaultWindowBase (const String& appName, const juce::Point< int >& initSize);

protected:
    void init();

private:
    void         closeButtonPressed() final;
    juce::Colour getBackgroundColour();
};


template < typename ComponentType >
struct DefaultWindow : public DefaultWindowBase
{
    DefaultWindow (const String& appName, const juce::Point< int >& initSize)
        : DefaultWindowBase (appName, initSize)
    {
        setSize (initSize.x, initSize.y);
        setContentOwned (new ComponentType(), false);
        init();
    }
};

}  // namespace bav
