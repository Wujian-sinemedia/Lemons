
namespace lemons::gui
{
TextButton::TextButton (const String& buttonText, std::function<void()> whenClicked)
    : juce::TextButton (TRANS (buttonText), buttonText)
{
	onClick = std::move (whenClicked);

	this->setName (buttonText);
	this->setComponentID (buttonText);
}


ToggleTextButton::ToggleTextButton (const String& buttonText, std::function<void (bool)> whenClicked)
    : TextButton (buttonText, [whenClicked, this]
                  { whenClicked (getToggleState()); })
{
}


ImageButton::ImageButton (const juce::Image& image, std::function<void()> whenClicked, float opacityRange)
{
	jassert (opacityRange >= 0.f && opacityRange <= 1.f);

	onClick = std::move (whenClicked);

	this->setImages (false, true, true,
	                 image, 1.f - opacityRange, {},
	                 {}, 1.f, {},
	                 {}, 1.f, juce::Colours::lightgrey);
}


}  // namespace lemons::gui
