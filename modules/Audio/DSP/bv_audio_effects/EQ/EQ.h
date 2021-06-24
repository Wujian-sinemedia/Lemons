
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class EQ : public AudioEffect< SampleType >
{
public:
    using Filter = Filter< SampleType >;

    EQ() = default;
    EQ (int initNumFilters);

    void process (juce::AudioBuffer< SampleType >& audio) final;

    void prepare (double samplerate, int blocksize) final;

    /* This function may allocate or deallocate filters! */
    void setNumFilters (int numFilters);
    int  getNumFilters() const;

    Filter* getFilter (int index);
    Filter* getFilterOfType (typename Filter::FilterType type);  // returns the first filter found with the given type, else nullptr

private:
    juce::OwnedArray< Filter > filters;

    double lastSamplerate {44100.};
    int    lastBlocksize {512};
};

}  // namespace bav::dsp::FX
