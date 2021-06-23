#pragma once

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
