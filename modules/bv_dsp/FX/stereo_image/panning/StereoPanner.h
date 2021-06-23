
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class StereoPanner : public PannerBase
{
public:
    StereoPanner()          = default;
    virtual ~StereoPanner() = default;

    void prepare (int blocksize);
    void reset();

    void process (const SampleType* leftIn,
                  const SampleType* rightIn,
                  SampleType*       leftOut,
                  SampleType*       rightOut,
                  int               numSamples);

    void process (const juce::AudioBuffer< SampleType >& stereoInput,
                  juce::AudioBuffer< SampleType >&       stereoOutput);

private:
    SmoothedGain< SampleType, 1 > left, right;
};


/*
 A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
 */
template < typename SampleType >
class ReorderableStereoPanner : public StereoPanner< SampleType >,
                                public ReorderableEffect< SampleType >
{
protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) final;

    void fxChain_prepare (double, int blocksize) final;
};

}  // namespace bav::dsp::FX
