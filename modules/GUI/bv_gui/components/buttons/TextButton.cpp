
namespace bav::gui
{
TextButton::TextButton (const String& buttonText, std::function< void() > whenClicked, const String& tooltip)
    : juce::TextButton (buttonText, tooltip)
{
    onClick = whenClicked;
}

}  // namespace bav::gui
