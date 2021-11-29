namespace lemons
{

DeviceRotation::Listener::Listener (Callback&& callbackToUse)
    : callback (std::move (callbackToUse))
    , prevOrientation (juce::Desktop::getInstance().getCurrentOrientation())
{
#if JUCE_IOS || JUCE_ANDROID
	startTimerHz (60);
#endif
}

DeviceRotation::Listener::~Listener()
{
	if (isTimerRunning())
		stopTimer();
}

void DeviceRotation::Listener::timerCallback()
{
	const auto orientation = juce::Desktop::getInstance().getCurrentOrientation();

	if (orientation != prevOrientation)
	{
		callback (orientation);
		prevOrientation = orientation;
	}
}

/*-----------------------------------------------------------------------------------*/

juce::Desktop::DisplayOrientation DeviceRotation::getCurrent()
{
	return Desktop::getInstance().getCurrentOrientation();
}

void DeviceRotation::disable()
{
	auto& desktop = Desktop::getInstance();
	desktop.setOrientationsEnabled (desktop.getCurrentOrientation());
}

void DeviceRotation::enableOnlyVertical()
{
	Desktop::getInstance().setOrientationsEnabled (Desktop::upright | Desktop::upsideDown);
}

void DeviceRotation::enableOnlyLandscape()
{
	Desktop::getInstance().setOrientationsEnabled (Desktop::rotatedClockwise | Desktop::rotatedAntiClockwise);
}

/*-----------------------------------------------------------------------------------*/

DeviceRotation::ScopedDisabler::ScopedDisabler()
    : prevOrientations (juce::Desktop::getInstance().getOrientationsEnabled())
{
	DeviceRotation::disable();
}

DeviceRotation::ScopedDisabler::~ScopedDisabler()
{
	juce::Desktop::getInstance().setOrientationsEnabled (prevOrientations);
}

}  // namespace lemons
