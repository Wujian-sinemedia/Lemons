namespace lemons::gui
{
PopupComponent::PopupComponent (std::function<void()> toClose, bool useCloseButton, bool escapeKeyCloses)
    : closeFunc (std::move (toClose)), escapeKeyDestroys(escapeKeyCloses)
{
	setWantsKeyboardFocus (true);
    
    if (useCloseButton)
    {
        closeButton.reset (new TextButton ("X", closeFunc));
        addAndMakeVisible (closeButton.get());
    }
}

PopupComponent::PopupComponent (std::unique_ptr<PopupComponent>& holder, bool useCloseButton, bool escapeKeyCloses)
: PopupComponent ([&](){ holder.reset(); }, useCloseButton, escapeKeyCloses)
{
    
}

void PopupComponent::close() { closeFunc(); }

void PopupComponent::resized()
{
	grabKeyboardFocus();
	resizeTriggered();
    
    if (closeButton.get() != nullptr)
    {
        // position close button
    }
}

void PopupComponent::resizeTriggered() { }

bool PopupComponent::keyPressed (const juce::KeyPress& key)
{
    if (escapeKeyDestroys)
    {
        if (key.getKeyCode() == juce::KeyPress::escapeKey)
        {
            closeFunc();
            return true;
        }
    }

	return keyPressRecieved (key);
}

bool PopupComponent::keyPressRecieved (const juce::KeyPress&) { return false; }

}  // namespace lemons::gui
