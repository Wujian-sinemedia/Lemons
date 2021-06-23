#pragma once

namespace bav::dsp::filters
{
template < typename T >
struct Coefs : juce::Array< T >
{
    Coefs();
    Coefs& operator= (std::initializer_list< T > list);
};

/*--------------------------------------------------------------------------------------------------------------*/

template < typename NumericType >
struct Coefficients
{
    Coefficients();

    void makeFirstOrderLowPass (double sampleRate, NumericType frequency);
    void makeFirstOrderHighPass (double sampleRate, NumericType frequency);
    void makeFirstOrderAllPass (double sampleRate, NumericType frequency);

    void makeLowPass (double sampleRate, NumericType frequency);
    void makeLowPass (double sampleRate, NumericType frequency, NumericType Q);

    void makeHighPass (double sampleRate, NumericType frequency);
    void makeHighPass (double sampleRate, NumericType frequency, NumericType Q);

    void makeBandPass (double sampleRate, NumericType frequency);
    void makeBandPass (double sampleRate, NumericType frequency, NumericType Q);

    void makeNotch (double sampleRate, NumericType frequency);
    void makeNotch (double sampleRate, NumericType frequency, NumericType Q);

    void makeAllPass (double sampleRate, NumericType frequency);
    void makeAllPass (double sampleRate, NumericType frequency, NumericType Q);

    //==============================================================================
    /** Returns the coefficients for a low-pass shelf filter with variable Q and gain.
     The gain is a scale factor that the low frequencies are multiplied by, so values
     greater than 1.0 will boost the low frequencies, values less than 1.0 will
     attenuate them.
     */
    void makeLowShelf (double      sampleRate,
                       NumericType cutOffFrequency,
                       NumericType Q,
                       NumericType gainFactor);

    /** Returns the coefficients for a high-pass shelf filter with variable Q and gain.
     The gain is a scale factor that the high frequencies are multiplied by, so values
     greater than 1.0 will boost the high frequencies, values less than 1.0 will
     attenuate them.
     */
    void makeHighShelf (double      sampleRate,
                        NumericType cutOffFrequency,
                        NumericType Q,
                        NumericType gainFactor);

    /** Returns the coefficients for a peak filter centred around a
     given frequency, with a variable Q and gain.
     The gain is a scale factor that the centre frequencies are multiplied by, so
     values greater than 1.0 will boost the centre frequencies, values less than
     1.0 will attenuate them.
     */
    void makePeakFilter (double      sampleRate,
                         NumericType centreFrequency,
                         NumericType Q,
                         NumericType gainFactor);

    //==============================================================================
    /** Returns the filter order associated with the coefficients */
    int getFilterOrder() const noexcept;

    /** Returns the magnitude frequency response of the filter for a given frequency
     and sample rate
     */
    double getMagnitudeForFrequency (double frequency,
                                     double sampleRate) const noexcept;

    /** Returns the magnitude frequency response of the filter for a given frequency array
     and sample rate.
     */
    void getMagnitudeForFrequencyArray (const double* frequencies,
                                        double*       magnitudes,
                                        size_t        numSamples,
                                        double        sampleRate) const noexcept;

    /** Returns the phase frequency response of the filter for a given frequency and
     sample rate
     */
    double getPhaseForFrequency (double frequency, double sampleRate) const noexcept;

    /** Returns the phase frequency response of the filter for a given frequency array
     and sample rate.
     */
    void getPhaseForFrequencyArray (double* frequencies,
                                    double* phases,
                                    size_t  numSamples,
                                    double  sampleRate) const noexcept;

    /** Returns a raw data pointer to the coefficients. */
    NumericType* getRawCoefficients() noexcept;

    /** Returns a raw data pointer to the coefficients. */
    const NumericType* getRawCoefficients() const noexcept;

    //==============================================================================
    /** The raw coefficients.
     You should leave these numbers alone unless you really know what you're doing.
     */
    Coefs< NumericType > coefficients;

private:
    static constexpr NumericType inverseRootTwo =
        static_cast< NumericType > (0.70710678118654752440L);
};

}  // namespace bav::dsp::filters
