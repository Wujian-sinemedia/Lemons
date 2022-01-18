#pragma once

namespace New
{
class GUI : public plugin::GUI<State>
{
public:
	using plugin::GUI<State>::GUI;

	virtual ~GUI() override;

private:
	void paint (juce::Graphics& g) final;
	void resized() final;

	bool keyPressed (const juce::KeyPress& key) final;
	void modifierKeysChanged (const juce::ModifierKeys& modifiers) final;
	void focusLost (FocusChangeType cause) final;

	// gui::DarkModeSentinel darkModeSentinel {internals.guiDarkMode, *this};
};

}  // namespace New
