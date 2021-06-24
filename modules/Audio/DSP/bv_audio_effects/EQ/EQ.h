
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class EQ : public AudioEffect< SampleType >
{
public:
    using Filter = Filter< SampleType >;

    void process (juce::AudioBuffer< SampleType >& audio) final;

    void prepare (double samplerate, int blocksize) final;

    void addBand (Filter* newFilter);
    
    template<typename... Args>
    void addBand (Args&&... args)
    {
        addBand (new Filter (std::forward<Args>(args)...));
    }
    
    int getNumBands() const;

    Filter* getBand (int index);
    Filter* getBandOfType (FilterType type);  // returns the first filter found with the given type, else nullptr

private:
    juce::OwnedArray< Filter > filters;

    double lastSamplerate {44100.};
    int    lastBlocksize {512};
};

}  // namespace bav::dsp::FX
