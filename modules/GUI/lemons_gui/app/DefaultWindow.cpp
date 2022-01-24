
namespace lemons
{

[[nodiscard]] static inline juce::Colour getDefaultWindowBackgroundColor()
{
	return juce::Desktop::getInstance().getDefaultLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
}

DefaultWindowBase::DefaultWindowBase (const String& appName, const Dimensions& initialSize)
	: DocumentWindow (appName, getDefaultWindowBackgroundColor(), allButtons)
{
	jassert (initialSize.isValid());

	setUsingNativeTitleBar (true);
	setSize (initialSize.getWidth(), initialSize.getHeight());
}

void DefaultWindowBase::closeButtonPressed()
{
	juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

void DefaultWindowBase::init()
{
#if LEMONS_MOBILE
	setFullScreen (true);
#else
	setResizable (true, true);
	centreWithSize (getWidth(), getHeight());
#endif

	setVisible (true);
}

}  // namespace lemons
