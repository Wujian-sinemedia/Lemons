
namespace bav::gui
{

LevelMeter::LevelMeter (GainMeterParameter& meterToUse)
  : GainMeterParameter::Listener (meterToUse),
    meter (meterToUse)
{
    
}

juce::String LevelMeter::getTooltip()
{
    return meter.parameterNameShort;
}

void LevelMeter::paramValueChanged (float newValue)
{
    juce::ignoreUnused (newValue);
    repaint();
}



GainReductionMeter::GainReductionMeter (GainMeterParameter& meterToUse)
  : GainMeterParameter::Listener (meterToUse),
    meter (meterToUse)
{
    
}

juce::String GainReductionMeter::getTooltip()
{
    return meter.parameterNameShort;
}

void GainReductionMeter::paramValueChanged (float newValue)
{
    juce::ignoreUnused (newValue);
    repaint();
}


}  // namespace
