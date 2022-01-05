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

namespace lemons::math
{

/** @addtogroup lemons_math Math
    @ingroup lemons_core
    Math utility functions.

 @{
 */


/** Returns true if the number is divisible by the divisor with no remainder. */
[[nodiscard]] constexpr bool isDivisibleBy (int number, int divisor) noexcept
{
	return number % divisor == 0;
}


/** Returns true if the number is evenly divisible by 2. */
[[nodiscard]] constexpr bool numberIsEven (int number) noexcept
{
	return isDivisibleBy (number, 2);
}


/** Returns the period in samples of a frequency at the specified samplerate. */
template <typename FreqType>
[[nodiscard]] inline int periodInSamples (double samplerate, FreqType freqHz) noexcept
{
	jassert (freqHz > FreqType (0.));

	const auto val = samplerate / static_cast<double> (freqHz);

	return juce::roundToInt (val);
}

/** Returns the frequency in Hz with a given period in samples at the specified samplerate. */
template <typename PeriodType>
[[nodiscard]] inline PeriodType freqFromPeriod (double samplerate, PeriodType period) noexcept
{
	jassert (period > PeriodType (0.));

	const auto val = samplerate / static_cast<double> (period);

	if constexpr (std::is_same_v<PeriodType, int>)
		return juce::roundToInt (val);
	else
		return static_cast<PeriodType> (val);
}


/** Converts a number of samples to milliseconds at the specified samplerate. */
[[nodiscard]] constexpr double sampsToMs (double samplerate, int numSamples) noexcept
{
	jassert (samplerate > 0.);

	const auto val = static_cast<double> (numSamples) / samplerate * 1000.;

	return static_cast<double> (val);
}

/** Converts a number of milliseconds to the closest integer number of samples at the specified samplerate. */
template <typename msType>
[[nodiscard]] inline int msToSamps (double samplerate, msType ms) noexcept
{
	const auto val = samplerate / 1000. * static_cast<double> (ms);

	return juce::roundToInt (val);
}

/** Converts a MIDI note to a frequency in Hz. */
template <typename T>
[[nodiscard]] inline T midiToFreq (T midiNote) noexcept
{
	const auto val = 440. * std::pow (2., (static_cast<double> (midiNote) - 69.) / 12.);

	if constexpr (std::is_same_v<T, int>)
		return juce::roundToInt (val);
	else
		return static_cast<T> (val);
}

/** Converts a frequency in Hz to a MIDI note. */
template <typename T>
[[nodiscard]] inline T freqToMidi (T freqHz) noexcept
{
	const auto val = 69. + 12. * std::log2 (static_cast<double> (freqHz) / 440.);

	if constexpr (std::is_same_v<T, int>)
		return juce::roundToInt (val);
	else
		return static_cast<T> (val);
}

///@}

}  // namespace lemons::math
