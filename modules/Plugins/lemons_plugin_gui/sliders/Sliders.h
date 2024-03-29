
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
 * ======================================================================================
 */

#pragma once

#include "SliderBase.h"

namespace lemons::gui::components
{

/**
	Plugin slider that attaches to a float parameter
	@see plugin::FloatParameter
 */
class FloatSlider : public SliderBase, private plugin::FloatParameter::Listener
{
public:

	/**
		Creates a FloatSlider attached to a float parameter.
	*/
	explicit FloatSlider (plugin::FloatParameter& paramToUse, SliderStyle style = SliderStyle::LinearVertical);

	plugin::FloatParameter& param;

private:

	void paramValueChanged (float newValue) final;
	void paramDefaultChanged (float newDefault) final;

	void valueChanged() final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/**
	Plugin slider that attaches to an integer parameter
	@see plugin::IntParameter
 */
class IntSlider : public SliderBase, private plugin::IntParameter::Listener
{
public:

	/**
	Creates an IntSlider attached to an integer parameter.
	*/
	explicit IntSlider (plugin::IntParameter& paramToUse, SliderStyle style = SliderStyle::LinearVertical);

	plugin::IntParameter& param;

private:

	void paramValueChanged (int newValue) final;
	void paramDefaultChanged (int newDefault) final;

	void valueChanged() final;
};

}  // namespace lemons::gui::components
