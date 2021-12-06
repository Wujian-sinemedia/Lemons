#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace lemons
{
template <typename SampleType>
struct ValueSmoother : juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative>
{
    using juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative>::SmoothedValue;
    
	void set (SampleType newGain, bool snapImmediately = false);

	template <typename T>
	void set (T newGain, bool snapImmediately = false)
	{
		set (static_cast<SampleType> (newGain), snapImmediately);
	}
};

}  // namespace lemons
