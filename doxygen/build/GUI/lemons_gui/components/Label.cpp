
/** @ingroup components
 *  @{
 */

namespace lemons::gui
{
Label::Label (const String& textToUse)
    : juce::Label (textToUse, textToUse)
{
}

void Label::set (const String& newText)
{
    setText (newText, juce::NotificationType::dontSendNotification);
}
}  // namespace lemons::gui

/** @}*/
