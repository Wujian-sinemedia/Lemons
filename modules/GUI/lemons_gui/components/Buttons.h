/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

#pragma once

namespace lemons::gui::components
{

/** @defgroup lemons_gui_components Components
	@ingroup lemons_gui
	GUI components and widgets.
 */

/** @defgroup lemons_buttons Buttons
	@ingroup lemons_gui_components
	Button component classes.
 */

/** @ingroup lemons_buttons
	A simple wrapper around juce::TextButton that allows you to specify a lambda that will be called when the button is clicked.
	@see ToggleTextButton
 */
struct TextButton : juce::TextButton
{
	explicit TextButton (const String& buttonText, std::function<void()> whenClicked);
};


/** @ingroup lemons_buttons
	A simple wrapper around TextButton that functions as an on/off toggle and allows you to specify a lambda that will be called when the button is clicked.
	@see TextButton
 */
struct ToggleTextButton : TextButton
{
	explicit ToggleTextButton (const String& buttonText, bool initialValue, std::function<void (bool)> whenClicked);

	void set (bool value);
};


/** @ingroup lemons_buttons
	A simple wrapper around juce::ImageButton that allows you to specify a lambda that will be called when the button is clicked.
	@param image The image to use.
	@param whenClicked Lambda function that will be called when the button is clicked.
	@param opacityRange The difference in the image's opacity between when the mouse is pressed and when it's not
 */
struct ImageButton : juce::ImageButton
{
	explicit ImageButton (const juce::Image& image, std::function<void()> whenClicked, float opacityRange = 0.2f);
};

}  // namespace lemons::gui::components
