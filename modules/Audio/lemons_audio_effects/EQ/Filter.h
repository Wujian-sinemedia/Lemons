
#pragma once

namespace lemons::dsp::FX
{
/**
    An enum that represents the type of filter topology to be used by a Filter effect.
 */
enum FilterType
{
    LowPass,
    HighPass,
    LowShelf,
    HighShelf,
    BandPass,
    Notch,
    Peak,
    AllPass
};


/**
    A struct that holds the parameters for a Filter effect.
 */
struct FilterParams
{
    /** Constructs a FilterParams object using floats. */
    FilterParams (FilterType typeToUse, float freqToUse, float QToUse, float gainToUse);

    /** Constructs a FilterParams object using doubles. */
    FilterParams (FilterType typeToUse, double freqToUse, double QToUse, double gainToUse);

    FilterType type;
    float      freq, Q, gain;
};


/**
    A filter audio effect class.
    @see filters::Filter, filters::Multifilter
 */
template < typename SampleType >
class Filter : public AudioEffect< SampleType >
{
public:
    /** Creates a default filter. */
    Filter() = default;

    /** Creates a filter with specified parameters. */
    Filter (FilterType filterType, float frequency = 440.f, float Qfactor = 0.70710678118654752440f, float gainMult = 1.f);

    /** Creates a filter from a FilterParams object. */
    Filter (FilterParams params);

    /** Sets the type of this filter. */
    void setFilterType (FilterType newType);

    /** Returns the current type of this filter. */
    FilterType getFilterType() const;

    /** Sets the frequency of this filter. */
    void setFilterFrequency (float newFreqHz);

    /** Returns the current frequency of this filter. */
    float getFilterFrequency() const;

    /** Sets the Q factor of this filter. */
    void setQfactor (float newQ);

    /** Returns the current Q factor of this filter. */
    float getQfactor() const;

    /** Sets the gain of this filter.
        Note that gain is only relevant for shelf and peak filter types.
     */
    void setGain (float newGain);

    /** Returns the current gain of this filter.
        Note that gain is only relevant for shelf and peak filter types.
     */
    float getGain() const;

    /** Applies the filter to a stream of audio. */
    void process (juce::AudioBuffer< SampleType >& audio) final;

    /** Prepares the filter. */
    void prepare (double samplerate, int blocksize) final;

    /** Returns a FilterParams object representing the state of this filter. */
    FilterParams getParams() const;

    /** Applies a FilterParams object to the state of this filter. */
    void setParams (FilterParams params);

private:
    filters::MultiFilter< SampleType, 2 > filter;

    FilterType type {HighPass};
    SampleType freq {static_cast< SampleType > (440.)};
    SampleType Q {static_cast< SampleType > (0.70710678118654752440L)};
    SampleType gain {static_cast< SampleType > (1.)};

    double sampleRate {44100.};
};


/**
    A class that represents an audio engine holding a filter effect.
 */
template < typename SampleType >
struct FilterEngine : EffectEngine< Filter, SampleType >
{
    using EffectEngine< Filter, SampleType >::EffectEngine;

    /** Returns a reference to the underlying filter effect object. */
    Filter< SampleType >& getFilter()
    {
        return (*this)();
    }
};

}  // namespace lemons::dsp::FX
