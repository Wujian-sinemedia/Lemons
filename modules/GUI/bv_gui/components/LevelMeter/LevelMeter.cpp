
namespace bav::gui
{
LevelMeter::LevelMeter (GainMeterParameter& meterToUse,
                        bool isGainReduction,
                        juce::Colour fillColor,
                        bool drawBorder)
    : GainMeterParameter::Listener (meterToUse),
      meter (meterToUse),
      isInverted (isGainReduction),
      color (fillColor),
      border (drawBorder)
{
}

juce::String LevelMeter::getTooltip()
{
    return meter.parameterNameShort;
}

void LevelMeter::paramValueChanged (float)
{
    repaint();
}

void LevelMeter::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    
    if (border)
    {
        g.setColour (juce::Colours::black);
        g.drawRect (bounds);
    }
    
    const auto pcntFilled = juce::Decibels::decibelsToGain (meter.get());
    
    const auto startingY = isInverted ? 1.f - pcntFilled : 0.f;
    
    const auto scaledArea = scaleRect (bounds, {0.f, startingY, 1.f, pcntFilled});
    
    g.setColour (color);
    g.fillRect (scaledArea);
}


}  // namespace bav::gui
