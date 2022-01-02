#pragma once

namespace lemons::gui
{

/** @ingroup lemons_gui
    A simple wrapper around juce::Label that simplifies its API a bit.
 */
struct Label : juce::Label
{
	/** Creates a label. */
	Label (const String& textToUse = {});

	/** Changes the label's text. */
	void set (const String& newText);
};

}  // namespace lemons::gui
