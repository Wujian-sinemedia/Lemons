/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace lemons
{

/** @ingroup lemons_dsp
    A simple wrapper around juce::SmoothedValue with some less wordy template syntax, and a couple convenience functions.
 */
template <typename SampleType>
struct ValueSmoother final : juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative>
{
	/** Default constructor. */
	using juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative>::SmoothedValue;

	/** Sets the target output value, optionally snapping to it immediately. */
	void set (SampleType newGain, bool snapImmediately = false);

	/** Sets the target output value, optionally snapping to it immediately. */
	template <typename T>
	void set (T newGain, bool snapImmediately = false)
	{
		set (static_cast<SampleType> (newGain), snapImmediately);
	}
};

}  // namespace lemons
