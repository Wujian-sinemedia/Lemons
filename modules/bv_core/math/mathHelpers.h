

namespace bav::math
{
/*
        This namespace conatins several floating inlined functions that are useful helpers and utilities for math-related operations
    */


//  returns true a specified percent of the time
extern bool probability (int percentOfTheTime);

/* Returns true if the given integer number is even. */
extern bool isEven (int number) noexcept;

/* Returns true if the given integer number is odd. */
extern bool isOdd (int number) noexcept;


/* Checks to see if a number is NaN eg. sqrt (-1). */
template < typename Type >
extern bool isnan (Type value);

/* Checks to see if a number is Inf eg. 100.0 / 0.0. */
template < typename Type >
extern bool isinf (Type value);


// returns true if n is 2^something
template < typename Integer >
extern bool isPowerOfTwo (Integer n);


// returns the period in samples of a specified frequency in hz at a specified samplerate
template < typename FreqType >
extern int periodInSamples (double samplerate, FreqType freqHz);


// returns the resultant fundamental frequency in Hz from a specified period in samples and samplerate
template < typename PeriodType >
extern PeriodType freqFromPeriod (double     samplerate,
                                  PeriodType period);


// converts a specified number of samples to milliseconds
extern int sampsToMs (double samplerate, int numSamples);

// converts a specified amount of time in milliseconds to the closest integer number of samples at the specified samplerate
template < typename msType >
extern int msToSamps (double samplerate, msType ms);


// converts a midi note to a frequency in Hz
template < typename noteType >
extern noteType midiToFreq (noteType midiNote);


// converts a frequency in Hz to a midi pitch
template < typename noteType >
extern noteType freqToMidi (noteType freqHz);


}  // namespace bav::math
