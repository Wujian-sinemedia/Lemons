#pragma once

#include <lemons_serializing/lemons_serializing.h>
#include <lemons_mobile_utils/lemons_mobile_utils.h>

namespace lemons
{
/**
    Base class for a GUI app's main window.
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
    void closeButtonPressed() final;
};


/**
    A class representing a GUI app's main window.
    @tparam ComponentType The type of top-level main component that this window owns and displays. This must inherit from juce::Component and must be default-constructable.
    @see DefaultWindowBase, GuiApp
 */
template < typename ComponentType, LEMONS_MUST_INHERIT_FROM (ComponentType, juce::Component) >
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
