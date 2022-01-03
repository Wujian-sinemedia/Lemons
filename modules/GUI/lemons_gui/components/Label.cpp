
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


TextEntry::TextEntry (const String& initialText, std::function<void(const String&)> textChangeFunc)
: Label(initialText)
{
    setEditable (true, true, false);
    
    this->onTextChange = [=](){ textChangeFunc (getText (true)); };
}

}  // namespace lemons::gui
