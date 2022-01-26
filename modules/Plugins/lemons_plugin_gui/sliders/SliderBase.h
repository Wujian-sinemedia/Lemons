#pragma once

#include <lemons_gui/lemons_gui.h>
#include <lemons_plugin/lemons_plugin.h>

namespace lemons::gui::components
{
/**
	Base class for plugin sliders that handles some logic common to both int and float parameter sliders.
	You should never have to use this class directly, you should use FloatSlider or IntSlider in your project code.
	@see FloatSlider, IntSlider
 */
class SliderBase : public juce::Slider
{
public:

	/**
		Creates a slider that references a plugin parameter.
		The slider will map its range, default value, name, label, and some other properties to the parameter's settings. Your parameter should be fully constructed and configured before you attach it to a slider.
		@see plugin::Parameter
	*/
	explicit SliderBase (plugin::Parameter& paramToUse);

private:

	plugin::Parameter& param_;

	void startedDragging() final;
	void stoppedDragging() final;
};

}  // namespace lemons::gui::components
