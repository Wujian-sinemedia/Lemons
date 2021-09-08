
namespace bav::gui
{
SliderBase::SliderBase (plugin::Parameter& paramToUse)
    : param (paramToUse)
{
    const auto paramName = param.getName();
    Slider::setName (paramName);
    Slider::setComponentID (paramName);
    Slider::setTooltip (paramName);

    Slider::setTextBoxIsEditable (true);
    Slider::setTextValueSuffix (param.getLabel());

    auto range = param.getNormalisableRange();

    auto convertFrom0To1Function = [=] (double currentRangeStart,
                                        double currentRangeEnd,
                                        double normalisedValue) mutable
    {
        range.start = static_cast< float > (currentRangeStart);
        range.end   = static_cast< float > (currentRangeEnd);

        return static_cast< double > (range.convertFrom0to1 (static_cast< float > (normalisedValue)));
    };

    auto convertTo0To1Function = [=] (double currentRangeStart,
                                      double currentRangeEnd,
                                      double mappedValue) mutable
    {
        range.start = static_cast< float > (currentRangeStart);
        range.end   = static_cast< float > (currentRangeEnd);

        return static_cast< double > (range.convertTo0to1 (static_cast< float > (mappedValue)));
    };

    auto snapToLegalValueFunction = [=] (double currentRangeStart,
                                         double currentRangeEnd,
                                         double mappedValue) mutable
    {
        range.start = static_cast< float > (currentRangeStart);
        range.end   = static_cast< float > (currentRangeEnd);

        return static_cast< double > (range.snapToLegalValue (static_cast< float > (mappedValue)));
    };

    juce::NormalisableRange< double > newRange {static_cast< double > (range.start),
                                                static_cast< double > (range.end),
                                                std::move (convertFrom0To1Function),
                                                std::move (convertTo0To1Function),
                                                std::move (snapToLegalValueFunction)};

    newRange.interval      = range.interval;
    newRange.skew          = range.skew;
    newRange.symmetricSkew = range.symmetricSkew;

    Slider::setNormalisableRange (newRange);

    Slider::textFromValueFunction = [this] (double value)
    {
        return param.getTextForDenormalizedValue (static_cast< float > (value));
    };

    Slider::setPopupDisplayEnabled (true, false, this);
    Slider::setPopupMenuEnabled (true);
}

void SliderBase::startedDragging()
{
    param.beginGesture();
}

void SliderBase::stoppedDragging()
{
    param.endGesture();
}

}  // namespace bav::gui
