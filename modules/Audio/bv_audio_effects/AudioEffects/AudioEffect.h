#pragma once

#include <bv_dsp/bv_dsp.h>

namespace bav::dsp::FX
{
template < typename SampleType >
struct AudioEffect
{
    virtual ~AudioEffect() = default;

    virtual void process (AudioBuffer< SampleType >& audio) = 0;

    virtual void prepare (double samplerate, int blocksize);
    virtual void bypassedBlock (int numSamples);
};


template < typename SampleType >
struct SidechainableAudioEffect : AudioEffect< SampleType >
{
    virtual void process (AudioBuffer< SampleType >& inOut, const AudioBuffer< SampleType >& sidechain) = 0;

    /* Sidechains the signal to itself. */
    void process (AudioBuffer< SampleType >& audio) override;
};


/* An effect that reports either its level or its gain reduction for each channel */
template < typename SampleType >
struct LevelReportingAudioEffect : SidechainableAudioEffect< SampleType >
{
    void process (AudioBuffer< SampleType >& inOut, const AudioBuffer< SampleType >& sidechain) final;

    /* Sidechains the signal to itself. */
    void process (AudioBuffer< SampleType >& audio) final;

    /* Should return the channel's average level / gain reduction for this frame */
    virtual SampleType processChannel (int               channel,
                                       int               numSamples,
                                       SampleType*       signal,
                                       const SampleType* sidechain) = 0;

    SampleType getGainReduction (int channel) const;
    SampleType getAverageGainReduction() const;
    SampleType getLevel (int channel) const;
    SampleType getAverageLevel() const;

private:
    Array< SampleType > gainReductions;
};

}  // namespace bav::dsp::FX
