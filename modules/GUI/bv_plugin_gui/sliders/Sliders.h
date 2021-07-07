
#pragma once

#include "SliderBase.h"

namespace bav::gui
{
class FloatSlider : public SliderBase,
                    private FloatParameter::Listener
{
public:
    FloatSlider (FloatParameter& paramToUse, SliderStyle style = SliderStyle::LinearVertical);

    FloatParameter& param;

private:
    void paramValueChanged (float newValue) final;
    void paramDefaultChanged (float newDefault) final;

    void valueChanged() final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class IntSlider : public SliderBase,
                  private IntParameter::Listener
{
public:
    IntSlider (IntParameter& paramToUse, SliderStyle style = SliderStyle::LinearVertical);

    IntParameter& param;

private:
    void paramValueChanged (int newValue) final;
    void paramDefaultChanged (int newDefault) final;

    void valueChanged() final;
};

}  // namespace bav::gui
