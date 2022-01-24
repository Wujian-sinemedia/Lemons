namespace lemons::gui::components
{
PopupComponentBase::PopupComponentBase (std::function<void()> toClose, bool useCloseButton, bool escapeKeyCloses)
	: closeFunc (std::move (toClose))
	, escapeKeyDestroys (escapeKeyCloses)
{
	if (toClose == nullptr)
		toClose = [this]()
		{ delete this; };

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

	return [&holder]()
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

	if (closeButton.get() != nullptr)
	{
		// position close button
	}

	resizeTriggered();
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

	return keyPressReceived (key);
}

bool PopupComponentBase::keyPressReceived (const juce::KeyPress&) { return false; }

}  // namespace lemons::gui::components
