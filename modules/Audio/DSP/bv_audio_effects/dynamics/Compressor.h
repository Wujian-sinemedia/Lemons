
#pragma once

namespace bav::dsp::FX
{
/*
    Simple compressor that allows you to sidechain the signal.
*/

template < typename SampleType >
class Compressor
{
public:
    Compressor();
    virtual ~Compressor() = default;

    void setThreshold (float newThresh_dB);
    void setRatio (float newRatio);
    void setAttack (float attackMs);
    void setRelease (float releaseMs);

    void prepare (int blocksize, double samplerate, int numChannels);
    void reset();

    //  processes a signal with no external sidechain
    void process (juce::AudioBuffer< SampleType >& signalToCompress,
                  SampleType*                      gainReduction = nullptr);

    // processes a signal with an external sidechain. Use the same buffer in both arguments to sidechain a signal to itself.
    void process (const juce::AudioBuffer< SampleType >& sidechain,
                  juce::AudioBuffer< SampleType >&       signalToCompress,
                  SampleType*                            gainReduction = nullptr);

    //  processes a signal with an external sidechain. Omit the sidechain argument or pass a nullptr to sidechain the signal to itself.
    void process (const int         channel,
                  const int         numSamples,
                  SampleType*       signalToCompress,
                  const SampleType* sidechain     = nullptr,
                  SampleType*       gainReduction = nullptr);

    //  processes a single sample with no external sidechain
    SampleType processSample (int         channel,
                              SampleType  inputSample,
                              SampleType* gainReduction = nullptr);

    //  processes a single sample. Omit the sidechainSample argument or pass the same value in both args to use as a standard compressor (sidechaining the signal to itself).
    SampleType processSample (int         channel,
                              SampleType  inputSample,
                              SampleType  sidechainSample,
                              SampleType* gainReduction = nullptr);

private:
    void update();

    SampleType                                threshold, thresholdInverse, ratioInverse;
    juce::dsp::BallisticsFilter< SampleType > envelopeFilter;

    juce::dsp::ProcessSpec spec;

    double     sampleRate  = 44100.0;
    SampleType thresholddB = 0.0, ratio = 1.0, attackTime = 1.0, releaseTime = 100.0;
};


/*
        A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
    */
template < typename SampleType >
class ReorderableCompressor : public Compressor< SampleType >,
                              public ReorderableEffect< SampleType >
{
protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) final;

    void fxChain_prepare (double samplerate, int blocksize) final;
};

}  // namespace bav::dsp::FX
