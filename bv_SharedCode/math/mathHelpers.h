

namespace bav::math
{
    
    /*
        This namespace conatins several floating inlined functions that are useful helpers and utilities for math-related operations
    */
    
    
    //  returns true a specified percent of the time
    inline bool probability (int percentOfTheTime)
    {
        return juce::Random::getSystemRandom().nextInt (100) < percentOfTheTime;
    }
    
    
    // returns true if n is 2^something
    template<typename Integer>
    inline bool isPowerOfTwo (Integer n)
    {
        return n > 0 && (n & (n - 1)) == 0;
    }
    
    
    // returns the period in samples of a specified frequency in hz at a specified samplerate
    template<typename FreqType>
    inline int periodInSamples (double samplerate, FreqType freqHz)
    {
        jassert (freqHz > FreqType(0.0));
        return juce::roundToInt (samplerate / freqHz);
    }
    
    
    // returns the number of decimal digits needed to print a given unsigned number
    inline int getNumDecimalDigits (uint32_t n)
    {
        return n < 1000 ? (n < 10 ? 1 : (n < 100 ? 2 : 3))
                        : n < 1000000 ? (n < 10000 ? 4 : (n < 100000 ? 5 : 6))
                        : n < 100000000  ? (n < 10000000 ? 7 : 8)
                        : n < 1000000000 ? 9 : 10;
    }
    
    
    // returns the resultant fundamental frequency in Hz from a specified period in samples and samplerate
    template<typename PeriodType>
    inline PeriodType freqFromPeriod (double samplerate, PeriodType period)
    {
        jassert (period > PeriodType(0.0));
        
        if constexpr (std::is_same_v<PeriodType, int>)
            return juce::roundToInt (samplerate / period);
        else
            return PeriodType(samplerate / period);
    }
    
    
    // converts a specified number of samples to milliseconds
    inline int sampsToMs (double samplerate, int numSamples)
    {
        jassert (samplerate > 0.0);
        return juce::roundToInt ((numSamples / samplerate) * 1000.0f);
    }
    
    
    // converts a specified amount of time in milliseconds to the closest integer number of samples at the specified samplerate
    template<typename msType>
    inline int msToSamps (double samplerate, msType ms)
    {
        return juce::roundToInt (samplerate / 1000.0f * ms);
    }
    
    
    // converts a midi note to a frequency in Hz
    template<typename noteType>
    inline noteType midiToFreq (noteType midiNote)
    {
        if constexpr (std::is_same_v<noteType, int>)
            return juce::roundToInt (440 * std::pow (2, (midiNote - 69)/12));
        else
            return noteType(440 * std::pow (2, (midiNote - 69)/12));
    }
    
    
    // converts a frequency in Hz to a midi pitch
    template<typename noteType>
    inline noteType freqToMidi (noteType freqHz)
    {
        if constexpr (std::is_same_v<noteType, int>)
            return juce::roundToInt (69 + 12 * log2(freqHz / 440));
        else
            return noteType(69 + 12 * log2(freqHz / 440));
    }
    
    
}  // namespace
