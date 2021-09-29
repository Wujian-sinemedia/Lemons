
namespace lemons
{

static inline juce::Colour getDefaultWindowBackgroundColor()
{
    return juce::Desktop::getInstance().getDefaultLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
}

DefaultWindowBase::DefaultWindowBase (const String& appName, const juce::Point< int >& initSize)
    : DocumentWindow (appName, getDefaultWindowBackgroundColor(), allButtons)
{
    setUsingNativeTitleBar (true);
    setSize (initSize.x, initSize.y);
}

void DefaultWindowBase::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

void DefaultWindowBase::init()
{
    if (Platform::isMobile())
    {
        setFullScreen (true);
    }
    else
    {
        setResizable (true, true);
        centreWithSize (getWidth(), getHeight());
    }

    setVisible (true);
}

}  // namespace lemons
