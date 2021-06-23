
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType, size_t channels = 1 >
class SmoothedGain
{
    using Smoother =
        juce::SmoothedValue< SampleType, juce::ValueSmoothingTypes::Multiplicative >;

public:
    SmoothedGain();
    virtual ~SmoothedGain() = default;

    void setGain (float gain);

    void prepare (int blocksize);
    void reset();
    void skipSamples (int numSamples);

    void process (SampleType* samples, int numSamples, int channel, float newGain);
    void process (SampleType* samples, int numSamples, int channel);
    void process (juce::AudioBuffer< SampleType >& audio, float newGain);
    void process (juce::AudioBuffer< SampleType >& audio);

private:
    juce::OwnedArray< Smoother > smoothers;

    int lastBlocksize = 0;
};


/*
 A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
 */
template < typename SampleType >
class ReorderableSmoothedGain : public SmoothedGain< SampleType >,
                                public ReorderableEffect< SampleType >
{
protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) final;

    void fxChain_prepare (double, int blocksize) final;
};

}  // namespace bav::dsp::FX
