

namespace bav::math
{
    
    /*
        This namespace conatins several floating inlined functions that are useful helpers and utilities for math-related operations
    */
    
    
    
    //  returns true a specified percent of the time
    static inline bool probability (int percentOfTheTime)
    {
        return juce::Random::getSystemRandom().nextInt (100) < percentOfTheTime;
    }
    
    
    /* Returns true if the given integer number is even. */
    static inline bool isEven (int number) noexcept
    {
        return ! (number & 0x1);
    }
    
    /* Returns true if the given integer number is odd. */
    static inline bool isOdd (int number) noexcept
    {
        return number & 0x1;
    }
    
    
    /* Checks to see if a number is NaN eg. sqrt (-1). */
    template <typename Type>
    static inline bool isnan (Type value)
    {
#if BV_OSX
        return std::isnan (value);
#else
        volatile Type num = value;
        return num != num;
#endif
    }
    
    /* Checks to see if a number is Inf eg. 100.0 / 0.0. */
    template <typename Type>
    static inline bool isinf (Type value)
    {
#if BV_WINDOWS
        return ! _finite (value);
#else
        return std::isinf (value);
#endif
    }

    
    // returns true if n is 2^something
    template<typename Integer>
    static inline bool isPowerOfTwo (Integer n)
    {
        return n > 0 && (n & (n - 1)) == 0;
    }
    
    
    // returns the period in samples of a specified frequency in hz at a specified samplerate
    template<typename FreqType>
    static inline int periodInSamples (double samplerate, FreqType freqHz)
    {
        jassert (freqHz > FreqType(0.0));
        return juce::roundToInt (samplerate / freqHz);
    }

    
    // returns the resultant fundamental frequency in Hz from a specified period in samples and samplerate
    template<typename PeriodType>
    static inline PeriodType freqFromPeriod (double samplerate, PeriodType period)
    {
        jassert (period > PeriodType(0.0));
        
        if constexpr (std::is_same_v<PeriodType, int>)
            return juce::roundToInt (samplerate / period);
        else
            return PeriodType(samplerate / period);
    }
    
    
    // converts a specified number of samples to milliseconds
    static inline int sampsToMs (double samplerate, int numSamples)
    {
        jassert (samplerate > 0.0);
        return juce::roundToInt ((numSamples / samplerate) * 1000.0f);
    }
    
    
    // converts a specified amount of time in milliseconds to the closest integer number of samples at the specified samplerate
    template<typename msType>
    static inline int msToSamps (double samplerate, msType ms)
    {
        return juce::roundToInt (samplerate / 1000.0f * ms);
    }
    
    
    // converts a midi note to a frequency in Hz
    template<typename noteType>
    static inline noteType midiToFreq (noteType midiNote)
    {
        if constexpr (std::is_same_v<noteType, int>)
            return juce::roundToInt (440 * std::pow (2, (midiNote - 69)/12));
        else
            return noteType(440 * std::pow (2, (midiNote - 69)/12));
    }
    
    
    // converts a frequency in Hz to a midi pitch
    template<typename noteType>
    static inline noteType freqToMidi (noteType freqHz)
    {
        if constexpr (std::is_same_v<noteType, int>)
            return juce::roundToInt (69 + 12 * log2(freqHz / 440));
        else
            return noteType(69 + 12 * log2(freqHz / 440));
    }
    
    
}  // namespace
