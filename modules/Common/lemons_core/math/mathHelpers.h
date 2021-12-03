
#pragma once

namespace lemons::math
{

[[nodiscard]] constexpr bool numberIsEven (int number) noexcept
{
    return number % 2 == 0;
}

// returns the period in samples of a specified frequency in hz at a specified samplerate
template <typename FreqType>
[[nodiscard]] int periodInSamples (double samplerate, FreqType freqHz) noexcept
{
	jassert (freqHz > FreqType (0.0));
	return juce::roundToInt (samplerate / static_cast<double> (freqHz));
}

// returns the resultant fundamental frequency in Hz from a specified period in samples and samplerate
template <typename PeriodType>
[[nodiscard]] PeriodType freqFromPeriod (double     samplerate,
                                     PeriodType period) noexcept
{
	jassert (period > PeriodType (0.0));
    
    const auto val = samplerate / static_cast<double> (period);

	if constexpr (std::is_same_v<PeriodType, int>)
		return juce::roundToInt (val);
	else
		return static_cast<PeriodType> (val);
}


// converts a specified number of samples to milliseconds
[[nodiscard]] double sampsToMs (double samplerate, int numSamples) noexcept;

// converts a specified amount of time in milliseconds to the closest integer number of samples at the specified samplerate
template <typename msType>
[[nodiscard]] int msToSamps (double samplerate, msType ms) noexcept
{
    const auto val = samplerate / 1000. * static_cast<double> (ms);
    
	return juce::roundToInt (val);
}


// converts a midi note to a frequency in Hz
template <typename T>
[[nodiscard]] T midiToFreq (T midiNote) noexcept
{
    const auto val = 440. * std::pow (2., (static_cast<double>(midiNote) - 69.) / 12.);
    
	if constexpr (std::is_same_v<T, int>)
		return juce::roundToInt (val);
	else
        return static_cast<T>(val);
}


// converts a frequency in Hz to a midi pitch
template <typename T>
[[nodiscard]] T freqToMidi (T freqHz) noexcept
{
    const auto val = 69. + 12. * std::log2 (static_cast<double>(freqHz) / 440.);
    
	if constexpr (std::is_same_v<T, int>)
		return juce::roundToInt (val);
	else
        return static_cast<T>(val);
}


}  // namespace lemons::math


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

struct MathTests : public CoreTest
{
	MathTests();

private:
	void runTest() final;
};

static MathTests mathFunctionsTest;

}  // namespace lemons::tests

#endif
