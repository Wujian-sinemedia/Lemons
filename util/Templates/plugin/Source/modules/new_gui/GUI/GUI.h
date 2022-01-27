#pragma once

namespace New
{
class GUI final : public plugin::GUI<State>
{
public:

	using plugin::GUI<State>::GUI;

	~GUI() final;

private:

	void paint (juce::Graphics& g) final;
	void resized() final;

	bool keyPressed (const juce::KeyPress& key) final;
	void modifierKeysChanged (const juce::ModifierKeys& modifiers) final;
	void focusLost (FocusChangeType cause) final;

	// gui::DarkModeSentinel darkModeSentinel {internals.guiDarkMode, *this};
};

}  // namespace New
