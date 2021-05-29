
#pragma once

namespace bav::gui
{
class LevelMeter : public juce::Component,
                   public juce::TooltipClient,
                   private GainMeterParameter::Listener
{
public:
    LevelMeter (GainMeterParameter& meterToUse);

    GainMeterParameter& meter;

private:
    void paramValueChanged (float newValue) override final;

    juce::String getTooltip() override final;
};


class GainReductionMeter : public juce::Component,
                           public juce::TooltipClient,
                           private GainMeterParameter::Listener
{
public:
    GainReductionMeter (GainMeterParameter& meterToUse);

    GainMeterParameter& meter;

private:
    void paramValueChanged (float newValue) override final;

    juce::String getTooltip() override final;
};

}  // namespace bav::gui
