
#pragma once

namespace bav::gui
{

class LevelMeter : public juce::Component,
                   public juce::TooltipClient,
                   private GainMeterParameter::Listener
{
public:
    LevelMeter (GainMeterParameter& meterToUse,
                bool isGainReduction = false,
                juce::Colour fillColor = juce::Colours::maroon,
                bool drawBorder = true);

private:
    void paint (juce::Graphics& g) final;
    
    void paramValueChanged (float newValue) final;

    juce::String getTooltip() final;
    
    GainMeterParameter& meter;
    const bool isInverted;
    const juce::Colour color;
    const bool border;
};

}  // namespace bav::gui
