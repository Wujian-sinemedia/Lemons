
namespace lemons::gui
{
FloatSlider::FloatSlider (plugin::FloatParameter& paramToUse, SliderStyle style)
    : SliderBase (paramToUse),
      plugin::FloatParameter::Listener (paramToUse),
      param (paramToUse)
{
    Slider::valueFromTextFunction = [this] (const String& text)
    {
        return static_cast< double > (param.getValueForString (text));
    };

    Slider::setDoubleClickReturnValue (true, double (param.getDefault()));

    Slider::setNumDecimalPlacesToDisplay (2);

    Slider::setSliderStyle (style);
}

void FloatSlider::paramValueChanged (float newValue)
{
    Slider::setValue (static_cast< double > (newValue),
                      juce::NotificationType::dontSendNotification);
}

void FloatSlider::paramDefaultChanged (float newDefault)
{
    Slider::setDoubleClickReturnValue (true, static_cast< double > (newDefault));
}

void FloatSlider::valueChanged()
{
    param.set (static_cast< float > (Slider::getValue()));
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

IntSlider::IntSlider (plugin::IntParameter& paramToUse, SliderStyle style)
    : SliderBase (paramToUse),
      plugin::IntParameter::Listener (paramToUse),
      param (paramToUse)
{
    Slider::valueFromTextFunction = [this] (const String& text)
    {
        return static_cast< double > (param.getValueForString (text));
    };

    Slider::setDoubleClickReturnValue (true, double (param.getDefault()));

    Slider::setNumDecimalPlacesToDisplay (0);

    Slider::setSliderStyle (style);
}

void IntSlider::paramValueChanged (int newValue)
{
    Slider::setValue (static_cast< double > (newValue),
                      juce::NotificationType::dontSendNotification);
}

void IntSlider::paramDefaultChanged (int newDefault)
{
    Slider::setDoubleClickReturnValue (true, static_cast< double > (newDefault));
}

void IntSlider::valueChanged()
{
    param.set (juce::roundToInt (Slider::getValue()));
}

}  // namespace bav::gui
