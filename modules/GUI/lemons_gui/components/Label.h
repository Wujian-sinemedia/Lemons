#pragma once

namespace lemons::gui
{

/** @ingroup lemons_gui_components
    A simple wrapper around juce::Label that simplifies its API a bit.
 */
struct Label : juce::Label
{
	/** Creates a label. */
    explicit Label (const String& textToUse = {});

	/** Changes the label's text. */
	void set (const String& newText);
};


struct TextEntry : public Label
{
    explicit TextEntry (const String& initialText = {}, std::function<void(const String&)> textChangeFunc = nullptr);
};


// number box

}  // namespace lemons::gui
