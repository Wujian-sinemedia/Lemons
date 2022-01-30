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

namespace lemons::gui::components
{

struct Slider : public juce::Slider
{
	template <typename ValueType>
	explicit Slider (ranges::Range<ValueType> range,
					 ValueType initial, ValueType doubleClickReset);

	template <typename ValueType>
	explicit Slider (ValueType min, ValueType max,
					 ValueType initial, ValueType doubleClickReset);
};


struct Knob : public Slider
{
	template <typename ValueType>
	explicit Knob (ranges::Range<ValueType> range,
				   ValueType initial, ValueType doubleClickReset);

	template <typename ValueType>
	explicit Knob (ValueType min, ValueType max,
				   ValueType initial, ValueType doubleClickReset);
};

}  // namespace lemons::gui::components
