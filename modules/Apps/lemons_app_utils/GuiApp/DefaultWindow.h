#pragma once

#include <lemons_serializing/lemons_serializing.h>
#include <lemons_mobile_utils/lemons_mobile_utils.h>

namespace lemons
{
//==============================================================================
/**
    Base class for a GUI app's main window.
    @tags{Apps}
 */
struct DefaultWindowBase : public juce::DocumentWindow
{
public:
    /** Constructor. */
    DefaultWindowBase (const String& appName, const juce::Point< int >& initSize);

protected:
    /** Initializes the window. */
    void init();

private:
    /** This method is called when the user tries to close the window. */
    void closeButtonPressed() final;
};


//==============================================================================
/**
    A class representing a GUI app's main window, templated on the type of main component it displays.
    @see DefaultWindowBase, GuiApp
    @tags{Apps}
 */
template < typename ComponentType, BV_MUST_INHERIT_FROM (ComponentType, juce::Component) >
struct DefaultWindow : public DefaultWindowBase
{
    /** Constructor. */
    DefaultWindow (const String& appName, const juce::Point< int >& initSize)
        : DefaultWindowBase (appName, initSize)
    {
        setSize (initSize.x, initSize.y);
        setContentOwned (new ComponentType(), false);
        init();
    }
};

}  // namespace lemons
