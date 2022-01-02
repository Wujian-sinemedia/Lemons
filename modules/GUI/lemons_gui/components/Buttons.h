#pragma once

namespace lemons::gui
{

/** @defgroup lemons_buttons Buttons
    @ingroup lemons_gui
    Button component classes.
 */

/** @ingroup lemons_buttons
    A simple wrapper around juce::TextButton that allows you to specify a lambda that will be called when the button is clicked.
    @see ToggleTextButton
 */
struct TextButton : juce::TextButton
{
	TextButton (const String& buttonText, std::function<void()> whenClicked);
};


/** @ingroup lemons_buttons
    A simple wrapper around TextButton that functions as an on/off toggle and allows you to specify a lambda that will be called when the button is clicked.
    @see TextButton
 */
struct ToggleTextButton : TextButton
{
	ToggleTextButton (const String& buttonText, std::function<void (bool)> whenClicked);
};


/** @ingroup lemons_buttons
    A simple wrapper around juce::ImageButton that allows you to specify a lambda that will be called when the button is clicked.
    @param image The image to use.
    @param whenClicked Lambda function that will be called when the button is clicked.
    @param opacityRange The difference in the image's opacity between when the mouse is pressed and when it's not
 */
struct ImageButton : juce::ImageButton
{
	ImageButton (const juce::Image& image, std::function<void()> whenClicked, float opacityRange = 0.2f);
};

}  // namespace lemons::gui
