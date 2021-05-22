
namespace bav::math
{

bool probability (int percentOfTheTime)
{
    return juce::Random::getSystemRandom().nextInt (100) < percentOfTheTime;
}


bool isEven (int number) noexcept
{
    return ! (number & 0x1);
}

bool isOdd (int number) noexcept
{
    return number & 0x1;
}



template < typename Type >
bool isnan (Type value)
{
#if JUCE_IOS || JUCE_MAC
    return std::isnan (value);
#else
    volatile Type num = value;
    return num != num;
#endif
}


template < typename Type >
bool isinf (Type value)
{
#if JUCE_WINDOWS
    return ! _finite (value);
#else
    return std::isinf (value);
#endif
}



template < typename Integer >
bool isPowerOfTwo (Integer n)
{
    return n > 0 && (n & (n - 1)) == 0;
}



template < typename FreqType >
int periodInSamples (double samplerate, FreqType freqHz)
{
    jassert (freqHz > FreqType (0.0));
    return juce::roundToInt (samplerate / freqHz);
}



template < typename PeriodType >
PeriodType freqFromPeriod (double     samplerate,
                                                  PeriodType period)
{
    jassert (period > PeriodType (0.0));
    
    if constexpr (std::is_same_v< PeriodType, int >)
        return juce::roundToInt (samplerate / period);
    else
        return PeriodType (samplerate / period);
}



int sampsToMs (double samplerate, int numSamples)
{
    jassert (samplerate > 0.0);
    return juce::roundToInt ((numSamples / samplerate) * 1000.0f);
}



template < typename msType >
int msToSamps (double samplerate, msType ms)
{
    return juce::roundToInt (samplerate / 1000.0f * ms);
}



template < typename noteType >
noteType midiToFreq (noteType midiNote)
{
    if constexpr (std::is_same_v< noteType, int >)
        return juce::roundToInt (440 * std::pow (2, (midiNote - 69) / 12));
    else
        return noteType (440 * std::pow (2, (midiNote - 69) / 12));
}



template < typename noteType >
noteType freqToMidi (noteType freqHz)
{
    if constexpr (std::is_same_v< noteType, int >)
        return juce::roundToInt (69 + 12 * log2 (freqHz / 440));
    else
        return noteType (69 + 12 * log2 (freqHz / 440));
}


}  // namespace bav::math
