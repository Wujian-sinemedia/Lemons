
#pragma once

namespace lemons::dsp::FX
{
template < typename SampleType >
class EQ : public AudioEffect< SampleType >
{
public:
    using Band = FilterEngine< SampleType >;

    void process (AudioBuffer< SampleType >& audio) final;

    void prepare (double samplerate, int blocksize) final;

    void addBand (Band* newFilter);

    template < typename... Args >
    void addBand (Args&&... args)
    {
        addBand (new Band (std::forward< Args > (args)...));
    }

    int getNumBands() const;

    Filter< SampleType >* getBand (int index);
    Filter< SampleType >* getBandOfType (FilterType type);  // returns the first filter found with the given type, else nullptr

private:
    OwnedArray< Band > bands;

    double lastSamplerate {44100.};
    int    lastBlocksize {512};
};

}  // namespace bav::dsp::FX
