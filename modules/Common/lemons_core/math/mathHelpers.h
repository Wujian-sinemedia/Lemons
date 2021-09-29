
#pragma once

namespace lemons::math
{
//  returns true a specified percent of the time
bool probability (int percentOfTheTime) noexcept;


// returns the period in samples of a specified frequency in hz at a specified samplerate
template < typename FreqType >
constexpr int periodInSamples (double samplerate, FreqType freqHz) noexcept
{
    jassert (freqHz > FreqType (0.0));
    return juce::roundToInt (samplerate / freqHz);
}


// returns the resultant fundamental frequency in Hz from a specified period in samples and samplerate
template < typename PeriodType >
constexpr PeriodType freqFromPeriod (double     samplerate,
                                     PeriodType period) noexcept
{
    jassert (period > PeriodType (0.0));

    if constexpr (std::is_same_v< PeriodType, int >)
        return juce::roundToInt (samplerate / period);
    else
        return PeriodType (samplerate / period);
}


// converts a specified number of samples to milliseconds
int sampsToMs (double samplerate, int numSamples) noexcept;

// converts a specified amount of time in milliseconds to the closest integer number of samples at the specified samplerate
template < typename msType >
constexpr int msToSamps (double samplerate, msType ms) noexcept
{
    return juce::roundToInt (samplerate / 1000.0f * ms);
}


// converts a midi note to a frequency in Hz
template < typename noteType >
constexpr noteType midiToFreq (noteType midiNote) noexcept
{
    if constexpr (std::is_same_v< noteType, int >)
        return juce::roundToInt (440 * std::pow (2, (midiNote - 69) / 12));
    else
        return noteType (440 * std::pow (2, (midiNote - 69) / 12));
}


// converts a frequency in Hz to a midi pitch
template < typename noteType >
constexpr noteType freqToMidi (noteType freqHz) noexcept
{
    if constexpr (std::is_same_v< noteType, int >)
        return juce::roundToInt (69 + 12 * log2 (freqHz / 440));
    else
        return noteType (69 + 12 * log2 (freqHz / 440));
}


}  // namespace lemons::math
