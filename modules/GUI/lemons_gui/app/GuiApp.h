#pragma once

namespace lemons
{

/** @ingroup lemons_gui_app
    Represents the base for a generic GUI app.
    @tparam ContentComponentType The type of top-level main component for this app's GUI. This must inherit from juce::Component and must be default-constructable.
    @see DefaultWindow, SystemInitializer
 */
template <typename ContentComponentType, LEMONS_MUST_INHERIT_FROM (ContentComponentType, juce::Component)>
class GuiApp : public juce::JUCEApplication
    , private locale::TranslationsInitializer
{
public:
	using WindowType = DefaultWindow<ContentComponentType>;

	/** Creates a default GuiApp. */
	explicit GuiApp (const String&     appName    = LEMONS_DEFAULT_APP_NAME,
	                 const String&     appVersion = LEMONS_DEFAULT_APP_VERSION,
	                 const Dimensions& initSize   = Dimensions::getDefault())
	    : initialSize (initSize)
	    , name (appName)
	    , version (appVersion)
	{
	}

	/** Called when the application starts.
	    @see shutdown, quit
	 */
	void initialise (const String&) final { window.reset (new WindowType (name, initialSize)); }

	/** Called to allow the application to clear up before exiting.
	 */
	void shutdown() final { window.reset(); }

	/** Returns the application's name. */
	const String getApplicationName() final { return name; }

	/** Returns the application's version number. */
	const String getApplicationVersion() final { return version; }

private:
	const Dimensions initialSize;

	std::unique_ptr<WindowType> window;

	const String name;
	const String version;
};

}  // namespace lemons
