#pragma once

namespace lemons
{
/**
    Represents the base for a generic GUI app.
    Templated on the type of component it owns.
    @see DefaultWindow, SystemInitializer
    @tags{Apps}
 */
template < typename ContentComponentType, BV_MUST_INHERIT_FROM (ContentComponentType, juce::Component) >
class GuiApp : public juce::JUCEApplication, private SystemInitializer
{
public:
    using WindowType = DefaultWindow< ContentComponentType >;
    using Size       = juce::Point< int >;

    /** Creates a default GuiApp. */
    GuiApp (const String& appName, const String& appVersion, const Size& initSize)
        : initialSize (initSize), name (appName), version (appVersion)
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
    const Size initialSize;

    std::unique_ptr< WindowType > window;

    const String name;
    const String version;
};

}  // namespace lemons
