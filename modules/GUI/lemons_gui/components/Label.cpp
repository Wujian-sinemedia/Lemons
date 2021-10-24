
namespace lemons::gui
{
Label::Label (const String& textToUse)
    : juce::Label (TRANS (textToUse), TRANS (textToUse))
{
}

void Label::set (const String& newText)
{
	setText (TRANS (newText), juce::NotificationType::dontSendNotification);
}
}  // namespace lemons::gui
