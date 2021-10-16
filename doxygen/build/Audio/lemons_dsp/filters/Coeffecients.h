
/** @ingroup filters
 *  @{
 */
#pragma once

namespace lemons::dsp::filters
{
/** A filter coefficients class, with lots of methods for creating various filter topologies.
    This class is designed so that coefficients can be recreated and reassigned during realtime playback; no allocations should ocur.
 */
template < typename NumericType >
struct Coefficients
{
    /** Creates a default coefficients class. */
    Coefficients();

    /** Creates the coefficients for a first order low pass filter. */
    void makeFirstOrderLowPass (double sampleRate, NumericType frequency);

    /** Creates the coefficients for a first order high pass filter. */
    void makeFirstOrderHighPass (double sampleRate, NumericType frequency);

    /** Creates the coefficients for a first order all-pass filter. */
    void makeFirstOrderAllPass (double sampleRate, NumericType frequency);

    /** Creates the coefficients for a low pass filter, with variable Q. */
    void makeLowPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);

    /** Creates the coefficients for a high pass filter, with variable Q. */
    void makeHighPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);

    /** Creates the coefficients for a band pass filter, with variable Q. */
    void makeBandPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);

    /** Creates the coefficients for a notch filter, with variable Q. */
    void makeNotch (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);

    /** Creates the coefficients for an all-pass filter, with variable Q. */
    void makeAllPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);

    //==============================================================================
    /** Creates the coefficients for a low-pass shelf filter with variable Q and gain.
        The gain is a scale factor that the low frequencies are multiplied by, so values greater than 1.0 will boost the low frequencies, values less than 1.0 will attenuate them.
     */
    void makeLowShelf (double      sampleRate,
                       NumericType cutOffFrequency,
                       NumericType Q          = inverseRootTwo,
                       NumericType gainFactor = (NumericType) 1);

    /** Creates the coefficients for a high-pass shelf filter with variable Q and gain.
        The gain is a scale factor that the high frequencies are multiplied by, so values greater than 1.0 will boost the high frequencies, values less than 1.0 will attenuate them.
     */
    void makeHighShelf (double      sampleRate,
                        NumericType cutOffFrequency,
                        NumericType Q          = inverseRootTwo,
                        NumericType gainFactor = (NumericType) 1);

    /** Creates the coefficients for a peak filter centred around a given frequency, with a variable Q and gain.
        The gain is a scale factor that the centre frequencies are multiplied by, so values greater than 1.0 will boost the centre frequencies, values less than 1.0 will attenuate them.
     */
    void makePeakFilter (double      sampleRate,
                         NumericType centreFrequency,
                         NumericType Q          = inverseRootTwo,
                         NumericType gainFactor = (NumericType) 1);

    //==============================================================================
    /** Returns the filter order associated with the coefficients */
    int getFilterOrder() const noexcept;

    NumericType*       getRawCoefficients() noexcept;
    const NumericType* getRawCoefficients() const noexcept;

    //==============================================================================

    /** The actual storage of the coefficient values.
        You should leave this alone unless you really know what you're doing.
     */
    struct Storage : juce::Array< NumericType >
    {
        Storage();
        Storage& operator= (std::initializer_list< NumericType > list);
    };

    //==============================================================================
    /** The raw coefficients.
        You should leave these numbers alone unless you really know what you're doing.
     */
    Storage coefficients;

private:
    static constexpr NumericType inverseRootTwo {static_cast< NumericType > (0.70710678118654752440L)};
};

}  // namespace lemons::dsp::filters

/** @}*/
