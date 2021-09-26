#pragma once

#include <lemons_plugin_parameters/lemons_plugin_parameters.h>
#include <lemons_gui/lemons_gui.h>

namespace bav::gui
{
class SliderBase : public juce::Slider
{
public:
    SliderBase (plugin::Parameter& paramToUse);

private:
    plugin::Parameter& param;

    void startedDragging() final;
    void stoppedDragging() final;
};

}  // namespace bav::gui
