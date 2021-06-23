#pragma once

#include <bv_plugin/bv_plugin.h>
#include <bv_gui/bv_gui.h>

namespace bav::gui
{
class SliderBase : public juce::Slider
{
public:
    SliderBase (Parameter& paramToUse);

private:
    Parameter& param;

    void startedDragging() final;
    void stoppedDragging() final;
};

}  // namespace bav::gui
