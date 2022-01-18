namespace New
{

GUI::~GUI()
{
	setLookAndFeel (nullptr);
}

void GUI::paint (juce::Graphics&)
{
}

void GUI::resized()
{
}

bool GUI::keyPressed (const juce::KeyPress&)
{
	return false;
}

void GUI::modifierKeysChanged (const juce::ModifierKeys&)
{
}

void GUI::focusLost (FocusChangeType)
{
}

}  // namespace New
