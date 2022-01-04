
namespace lemons::gui::components
{
Label::Label (const String& textToUse)
    : juce::Label (TRANS (textToUse), TRANS (textToUse))
{
}

void Label::set (const String& newText)
{
	setText (TRANS (newText), juce::NotificationType::dontSendNotification);
}


TextEntry::TextEntry (const String& initialText, std::function<void (const String&)> textChangeFunc)
    : Label (initialText)
{
	setEditable (true, true, false);

	this->onTextChange = [=]()
	{ textChangeFunc (getText (true)); };
}



static inline String getCompilationDateAsString()
{
    const auto compTime = juce::Time::getCompilationDate();
    
    return compTime.toString (true, true, false, true);
}

std::unique_ptr<Label> createCompilationDateViewer()
{
    return std::make_unique<Label> (getCompilationDateAsString());
}

}  // namespace lemons::gui
