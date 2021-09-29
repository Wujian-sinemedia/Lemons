
#pragma once

namespace lemons
{
template < typename SampleType >
struct ValueSmoother : juce::SmoothedValue< SampleType, juce::ValueSmoothingTypes::Multiplicative >
{
    void set (SampleType newGain, bool snapImmediately = false);

    template < typename T >
    void set (T newGain, bool snapImmediately = false)
    {
        set (static_cast< SampleType > (newGain), snapImmediately);
    }
};

}  // namespace bav
