
#pragma once

namespace bav::dsp::FX
{
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

struct FilterParams
{
    FilterParams (FilterType typeToUse, float freqToUse, float QToUse, float gainToUse);

    FilterParams (FilterType typeToUse, double freqToUse, double QToUse, double gainToUse);

    FilterType type;
    float      freq, Q, gain;
};

template < typename SampleType >
class Filter : public AudioEffect< SampleType >
{
public:
    Filter() = default;
    Filter (FilterType filterType, float frequency = 440.f, float Qfactor = 0.70710678118654752440f, float gainMult = 1.f);
    Filter (FilterParams params);

    void       setFilterType (FilterType newType);
    FilterType getFilterType() const;

    void  setFilterFrequency (float newFreqHz);
    float getFilterFrequency() const;

    void  setQfactor (float newQ);
    float getQfactor() const;

    /* Note that gain is only relevant for shelf and peak filter types. */
    void  setGain (float newGain);
    float getGain() const;

    void process (juce::AudioBuffer< SampleType >& audio) final;
    void prepare (double samplerate, int blocksize) final;

    FilterParams getParams() const;
    void         setParams (FilterParams params);

private:
    filters::MultiFilter< SampleType, 2 > filter;

    FilterType type {HighPass};
    SampleType freq {static_cast< SampleType > (440.)};
    SampleType Q {static_cast< SampleType > (0.70710678118654752440L)};
    SampleType gain {static_cast< SampleType > (1.)};

    double sampleRate {44100.};
};


template < typename SampleType >
struct FilterEngine : EffectEngine< Filter, SampleType >
{
    using EffectEngine< Filter, SampleType >::EffectEngine;

    Filter< SampleType >& getFilter()
    {
        return (*this) ();
    }
};

}  // namespace bav::dsp::FX
