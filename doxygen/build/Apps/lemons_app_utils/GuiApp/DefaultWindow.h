
/** @ingroup GuiApp
 *  @{
 */
#pragma once

#include <lemons_core/lemons_core.h>

namespace lemons
{
/**
    Base class for a GUI app's main window.
 */
struct DefaultWindowBase : public juce::DocumentWindow
{
public:
    /** Constructs a window with a specified initial size. */
    DefaultWindowBase (const String&             appName  = JUCE_APPLICATION_NAME_STRING,
                       const juce::Point< int >& initSize = defaultWindowSize());

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
    /** Constructs a default window. */
    DefaultWindow (const String&             appName  = JUCE_APPLICATION_NAME_STRING,
                   const juce::Point< int >& initSize = defaultWindowSize())
        : DefaultWindowBase (appName, initSize)
    {
        setSize (initSize.x, initSize.y);
        setContentOwned (new ComponentType(), false);
        init();
    }
};

}  // namespace lemons

/** @}*/
