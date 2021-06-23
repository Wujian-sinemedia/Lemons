
#pragma once

#include "PannerBase.h"

namespace bav::dsp::FX
{
template < typename SampleType >
class MonoToStereoPanner : public PannerBase
{
public:
    MonoToStereoPanner()          = default;
    virtual ~MonoToStereoPanner() = default;

    void prepare (int blocksize);
    void reset();

    void process (const SampleType* input,
                  SampleType*       leftOut,
                  SampleType*       rightOut,
                  int               numSamples);

    void process (const juce::AudioBuffer< SampleType >& monoInput,
                  juce::AudioBuffer< SampleType >&       stereoOutput);

private:
    SmoothedGain< SampleType, 1 > left, right;
};

}  // namespace bav::dsp::FX
