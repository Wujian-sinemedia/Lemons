
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class Filter : public AudioEffect< SampleType >
{
public:
    enum FilterType
    {
        LowPass,
        HighPass,
        LowShelf,
        HighShelf,
        BandPass,
        Notch,
        AllPass
    };

    void       setFilterType (FilterType newType);
    FilterType getFilterType() const { return type; }

    void  setFilterFrequency (float newFreqHz);
    float getFilterFrequency() const;

    void  setQfactor (float newQ);
    float getQfactor() const;

    /* Note that gain is only relevant for high & low shelf filter types. */
    void  setGain (float newGain);
    float getGain() const;

    void process (juce::AudioBuffer< SampleType >& audio) final;
    void prepare (double samplerate, int blocksize) final;

private:
    filters::MultiFilter< SampleType, 2 > filter;

    FilterType type {HighPass};
    SampleType freq {(SampleType) 440.};
    SampleType Q {static_cast< SampleType > (0.70710678118654752440L)};
    SampleType gain {(SampleType) 1.};

    double sampleRate {44100.};
};

}  // namespace bav::dsp::FX
