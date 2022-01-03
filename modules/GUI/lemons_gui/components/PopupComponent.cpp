namespace lemons::gui
{
PopupComponentBase::PopupComponentBase (std::function<void()> toClose, bool useCloseButton, bool escapeKeyCloses)
    : closeFunc (std::move (toClose))
    , escapeKeyDestroys (escapeKeyCloses)
{
	setWantsKeyboardFocus (true);

	if (useCloseButton)
	{
		closeButton.reset (new TextButton ("X", closeFunc));
		addAndMakeVisible (closeButton.get());
	}
}


static inline std::function<void()> makeCloserLambda (std::unique_ptr<PopupComponentBase>& holder, PopupComponentBase* parent)
{
	jassert (holder.get() == parent);

	return [&]()
	{ holder.reset(); };
}

PopupComponentBase::PopupComponentBase (std::unique_ptr<PopupComponentBase>& holder, bool useCloseButton, bool escapeKeyCloses)
    : PopupComponentBase (makeCloserLambda (holder, this), useCloseButton, escapeKeyCloses)
{
}

void PopupComponentBase::close() const { closeFunc(); }

void PopupComponentBase::resized()
{
	grabKeyboardFocus();
	resizeTriggered();

	if (closeButton.get() != nullptr)
	{
		// position close button
	}
}

void PopupComponentBase::resizeTriggered() { }

bool PopupComponentBase::keyPressed (const juce::KeyPress& key)
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

bool PopupComponentBase::keyPressRecieved (const juce::KeyPress&) { return false; }

}  // namespace lemons::gui
