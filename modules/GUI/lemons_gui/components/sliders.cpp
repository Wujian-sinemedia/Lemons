namespace lemons::gui::components
{

using SliderRange = ranges::Range<double>;

template <typename T>
static inline SliderRange convertToSliderRange (const ranges::Range<T>& range)
{
	return { static_cast<double> (range.start), static_cast<double> (range.end), static_cast<double> (range.interval), static_cast<double> (range.skew), range.symmetricSkew };
}

template <typename ValueType>
Slider::Slider (ValueType min, ValueType max,
				ValueType initial, ValueType doubleClickReset)
	: Slider (ranges::create (min, max), static_cast<float> (initial), static_cast<float> (doubleClickReset))
{
}

template Slider::Slider (float, float, float, float);
template Slider::Slider (int, int, int, int);
template Slider::Slider (double, double, double, double);


template <typename ValueType>
Slider::Slider (ranges::Range<ValueType> range,
				ValueType initial, ValueType doubleClickReset)
{
	setNormalisableRange (convertToSliderRange (range));

	setDoubleClickReturnValue (true, static_cast<double> (doubleClickReset));

	setSliderSnapsToMousePosition (true);
	setScrollWheelEnabled (true);
	setPopupMenuEnabled (true);
	setNumDecimalPlacesToDisplay (2);

	setTextBoxStyle (TextBoxBelow, false, getWidth() / 2, getHeight() / 8);

	setValue (static_cast<double> (initial));
}

template Slider::Slider (ranges::Range<float>, float, float);
template Slider::Slider (ranges::Range<int>, int, int);
template Slider::Slider (ranges::Range<double>, double, double);


template <typename ValueType>
Knob::Knob (ValueType min, ValueType max,
			ValueType initial, ValueType doubleClickReset)
	: Knob (ranges::create (min, max), static_cast<float> (initial), static_cast<float> (doubleClickReset))
{
}

template Knob::Knob (float, float, float, float);
template Knob::Knob (int, int, int, int);
template Knob::Knob (double, double, double, double);


template <typename ValueType>
Knob::Knob (ranges::Range<ValueType> range,
			ValueType initial, ValueType doubleClickReset)
	: Slider (range, initial, doubleClickReset)
{
	setSliderStyle (Rotary);

	RotaryParameters params;
	params.stopAtEnd = true;

	setRotaryParameters (params);

	setSliderSnapsToMousePosition (false);
}

template Knob::Knob (ranges::Range<float>, float, float);
template Knob::Knob (ranges::Range<int>, int, int);
template Knob::Knob (ranges::Range<double>, double, double);

}  // namespace lemons::gui::components
