
#pragma once

namespace bav::dsp::FX
{
/*
        Simple noise gate that allows you to sidechain the signal.
    */

template < typename SampleType >
class NoiseGate
{
public:
    NoiseGate();
    virtual ~NoiseGate() = default;

    void setInverted (bool gateBehaviorShouldBeInverted);
    void setThreshold (float newThreshold_dB);
    void setRatio (SampleType newRatio);
    void setAttack (SampleType newAttack_ms);
    void setRelease (SampleType newRelease_ms);

    void prepare (int numChannels, int maxBlocksize, double samplerate);
    void reset();

    //  processes a signal with no external sidechain
    void process (juce::AudioBuffer< SampleType >& signalToGate,
                  SampleType*                      gainReduction = nullptr);

    // processes a signal with an external sidechain. Use the same buffer in both arguments to sidechain a signal to itself.
    void process (const juce::AudioBuffer< SampleType >& sidechain,
                  juce::AudioBuffer< SampleType >&       signalToGate,
                  SampleType*                            gainReduction = nullptr);

    //  processes a signal with an external sidechain. Omit the sidechain argument or pass a nullptr to sidechain the signal to itself.
    void process (const int         channel,
                  const int         numSamples,
                  SampleType*       signalToGate,
                  const SampleType* sidechain     = nullptr,
                  SampleType*       gainReduction = nullptr);

    //  processes a single sample with no external sidechain
    SampleType processSample (const int        channel,
                              const SampleType sampleToGate,
                              SampleType*      gainReduction = nullptr);

    //  processes a single sample. Omit the sidechainSample argument or pass the same value in both args to use as a standard compressor (sidechaining the signal to itself).
    SampleType processSample (const int        channel,
                              const SampleType sampleToGate,
                              const SampleType sidechainValue,
                              SampleType*      gainReduction = nullptr);

private:
    void update();

    juce::dsp::ProcessSpec spec;

    SampleType                                threshold, thresholdInverse, currentRatio;
    juce::dsp::BallisticsFilter< SampleType > envelopeFilter, RMSFilter;

    SampleType thresholddB = -100, ratio = 10.0, attackTime = 1.0,
               releaseTime = 100.0;

    bool inverted = false;
};


/*
        A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
    */
template < typename SampleType >
class ReorderableNoiseGate : public NoiseGate< SampleType >,
                             public ReorderableEffect< SampleType >
{
protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) final;

    void fxChain_prepare (double samplerate, int blocksize) final;
};

}  // namespace bav::dsp::FX
