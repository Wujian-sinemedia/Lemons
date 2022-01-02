#pragma once

//#include <lemons_core/lemons_core.h>
//#include <juce_gui_basics/juce_gui_basics.h>

namespace lemons
{

#ifdef JUCE_APPLICATION_NAME_STRING
#  define LEMONS_DEFAULT_APP_NAME JUCE_APPLICATION_NAME_STRING
#elifdef JucePlugin_Name
#  define LEMONS_DEFAULT_APP_NAME JucePlugin_Name
#else
#  define LEMONS_DEFAULT_APP_NAME "LemonsApp"
#endif

#ifdef JUCE_APPLICATION_VERSION_STRING
#  define LEMONS_DEFAULT_APP_VERSION JUCE_APPLICATION_VERSION_STRING
#else
#  define LEMONS_DEFAULT_APP_VERSION "0.0.1"
#endif

/**
    Base class for a GUI app's main window.
 */
struct DefaultWindowBase : public juce::DocumentWindow
{
public:
	/** Constructs a window with a specified initial size. */
	DefaultWindowBase (const String&           appName  = LEMONS_DEFAULT_APP_NAME,
	                   const Dimensions& initialSize = Dimensions::getDefault());

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
template <typename ComponentType, LEMONS_MUST_INHERIT_FROM (ComponentType, juce::Component)>
struct DefaultWindow : public DefaultWindowBase
{
	/** Constructs a default window. */
	DefaultWindow (const String&           appName  = LEMONS_DEFAULT_APP_NAME,
                   const Dimensions& initialSize = Dimensions::getDefault())
	    : DefaultWindowBase (appName, initialSize)
	{
        jassert (initialSize.isValid());
		setSize (initialSize.getWidth(), initialSize.getHeight());
		setContentOwned (new ComponentType(), false);
		init();
	}
};

}  // namespace lemons
