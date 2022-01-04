#pragma once

namespace lemons::gui::components
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


/** @ingroup lemons_gui_components
    A simple text entry field, with a callback fired when the user changes the text.
 */
struct TextEntry : public Label
{
	explicit TextEntry (const String& initialText = {}, std::function<void (const String&)> textChangeFunc = nullptr);
};



/** Returns a Label displaying the compilation date of this module. */
std::unique_ptr<Label> createCompilationDateViewer();

// number box

}  // namespace lemons::gui
