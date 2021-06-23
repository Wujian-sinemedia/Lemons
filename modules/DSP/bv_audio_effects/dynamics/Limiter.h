
#pragma once

namespace bav::dsp::FX
{
/*
    Simple limiter that allows you to sidechain the signal.
*/

template < typename SampleType >
class Limiter
{
public:
    Limiter()          = default;
    virtual ~Limiter() = default;

    void setThreshold (float thresh_dB);
    void setRelease (float release_ms);

    void prepare (int blocksize, double samplerate, int numChannels);
    void reset();

    //  processes a signal with no sidechain
    void process (juce::AudioBuffer< SampleType >& signalToLimit,
                  SampleType*                      gainReduction = nullptr);

    //  processes a signal with an external sidechain. Use the same buffer in both arguments to sidechain a signal to itself.
    void process (const juce::AudioBuffer< SampleType >& sidechain,
                  juce::AudioBuffer< SampleType >&       signalToLimit,
                  SampleType*                            gainReduction = nullptr);

    //  processes a signal with an external sidechain. Omit the final argument or pass a nullptr to sidechain the signal to itself.
    void process (const int         channel,
                  const int         numSamples,
                  SampleType*       signalToLimit,
                  const SampleType* sidechain     = nullptr,
                  SampleType*       gainReduction = nullptr);


private:
    void update();

    Compressor< SampleType > firstStageCompressor, secondStageCompressor;

    juce::SmoothedValue< SampleType, juce::ValueSmoothingTypes::Linear >
        outputVolume;

    double sampleRate  = 44100.0;
    float  thresholddB = -10.0, releaseTime = 100.0;
};


/*
        A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
    */
template < typename SampleType >
class ReorderableLimiter : public Limiter< SampleType >,
                           public ReorderableEffect< SampleType >
{
protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) final;

    void fxChain_prepare (double samplerate, int blocksize) final;
};

}  // namespace bav::dsp::FX
