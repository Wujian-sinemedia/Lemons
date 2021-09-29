#pragma once

#include "DefaultWindow.h"

namespace lemons
{
template < typename ContentComponentType, BV_MUST_INHERIT_FROM (ContentComponentType, juce::Component) >
class GuiApp : public juce::JUCEApplication, private SystemInitializer
{
public:
    using WindowType = DefaultWindow< ContentComponentType >;

    GuiApp (const String& appName, const String& appVersion, const juce::Point< int >& initSize)
        : initialSize (initSize), name (appName), version (appVersion)
    {
    }

    void initialise (const String&) final { window.reset (new WindowType (name, initialSize)); }

    void shutdown() final { window.reset(); }

    const String getApplicationName() final { return name; }
    const String getApplicationVersion() final { return version; }

private:
    const juce::Point< int > initialSize;

    std::unique_ptr< WindowType > window;

    const String name;
    const String version;
};

}  // namespace lemons
