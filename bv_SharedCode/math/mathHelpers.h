

namespace bav::math
{
    
    /*
        This namespace conatins several floating inlined functions that are useful helpers and utilities for math-related operations
    */
    
    
    
/* MinGW doesn't appear to have sincos, so define it -- it's a single x87 instruction anyway  */
#if defined __GNUC__ && defined _WIN32
    static BV_FORCE_INLINE void sincos (double x, double* sin, double* cos)
    {
        __asm__ ("fsincos;" : "=t" (*cos), "=u" (*sin) : "0" (x) : "st(7)");
    }
    
    static BV_FORCE_INLINE void sincosf (float fx, float* fsin, float* fcos)
    {
        double sin, cos;
        sincos (fx, &sin, &cos);
        *fsin = float(sin);
        *fcos = float(cos);
    }
#endif
    
// alias __sincos as sincos and __sincosf as sincosf for GNU on Apple platforms
#ifndef LACK_SINCOS
  #if defined __GNUC__
    #if BV_APPLE
      #define sincosf __sincosf
      #define sincos __sincos
    #endif
  #endif
#endif
    
    
    //  returns true a specified percent of the time
    static BV_FORCE_INLINE bool probability (int percentOfTheTime)
    {
        return juce::Random::getSystemRandom().nextInt (100) < percentOfTheTime;
    }
    
    
    /* Returns true if the given integer number is even. */
    static BV_FORCE_INLINE bool isEven (int number) noexcept
    {
        return ! (number & 0x1);
    }
    
    /* Returns true if the given integer number is odd. */
    static BV_FORCE_INLINE bool isOdd (int number) noexcept
    {
        return number & 0x1;
    }
    
    
    /* Checks to see if a number is NaN eg. sqrt (-1). */
    template <typename Type>
    static BV_FORCE_INLINE bool isnan (Type value)
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
    static BV_FORCE_INLINE bool isinf (Type value)
    {
#if BV_WINDOWS
        return ! _finite (value);
#else
        return std::isinf (value);
#endif
    }

    
    // returns true if n is 2^something
    template<typename Integer>
    static BV_FORCE_INLINE bool isPowerOfTwo (Integer n)
    {
        return n > 0 && (n & (n - 1)) == 0;
    }
    
    
    // returns the period in samples of a specified frequency in hz at a specified samplerate
    template<typename FreqType>
    static BV_FORCE_INLINE int periodInSamples (double samplerate, FreqType freqHz)
    {
        jassert (freqHz > FreqType(0.0));
        return juce::roundToInt (samplerate / freqHz);
    }

    
    // returns the resultant fundamental frequency in Hz from a specified period in samples and samplerate
    template<typename PeriodType>
    static BV_FORCE_INLINE PeriodType freqFromPeriod (double samplerate, PeriodType period)
    {
        jassert (period > PeriodType(0.0));
        
        if constexpr (std::is_same_v<PeriodType, int>)
            return juce::roundToInt (samplerate / period);
        else
            return PeriodType(samplerate / period);
    }
    
    
    // converts a specified number of samples to milliseconds
    static BV_FORCE_INLINE int sampsToMs (double samplerate, int numSamples)
    {
        jassert (samplerate > 0.0);
        return juce::roundToInt ((numSamples / samplerate) * 1000.0f);
    }
    
    
    // converts a specified amount of time in milliseconds to the closest integer number of samples at the specified samplerate
    template<typename msType>
    static BV_FORCE_INLINE int msToSamps (double samplerate, msType ms)
    {
        return juce::roundToInt (samplerate / 1000.0f * ms);
    }
    
    
    // converts a midi note to a frequency in Hz
    template<typename noteType>
    static BV_FORCE_INLINE noteType midiToFreq (noteType midiNote)
    {
        if constexpr (std::is_same_v<noteType, int>)
            return juce::roundToInt (440 * std::pow (2, (midiNote - 69)/12));
        else
            return noteType(440 * std::pow (2, (midiNote - 69)/12));
    }
    
    
    // converts a frequency in Hz to a midi pitch
    template<typename noteType>
    static BV_FORCE_INLINE noteType freqToMidi (noteType freqHz)
    {
        if constexpr (std::is_same_v<noteType, int>)
            return juce::roundToInt (69 + 12 * log2(freqHz / 440));
        else
            return noteType(69 + 12 * log2(freqHz / 440));
    }
    
    
}  // namespace
