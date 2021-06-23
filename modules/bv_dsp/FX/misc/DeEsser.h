#pragma once

/*
    Simple de-esser that works by sending the input signal through an inverted noise gate sidechained by a hi-passed version of the original signal.
*/

namespace bav::dsp::FX
{
template < typename SampleType >
class DeEsser
{
public:
    DeEsser();
    virtual ~DeEsser() = default;

    void prepare (int blocksize, double samplerate);
    void reset();

    void setThresh (float newThresh_dB);
    void setDeEssAmount (int newAmount);

    void process (juce::AudioBuffer< SampleType >& audio,
                  SampleType*                      gainReduction = nullptr);

    void process (const int   channel,
                  SampleType* signalToDeEss,
                  const int   numSamples,
                  SampleType* gainReduction = nullptr);

    SampleType processSample (const int        channel,
                              const SampleType inputSample,
                              SampleType*      gainReduction = nullptr);

private:
    juce::dsp::IIR::Filter< SampleType > hiPass;

    NoiseGate< SampleType > gate;

    static constexpr double hiPassFreq = 7600.;
    static constexpr int    attackMs   = 20;
    static constexpr int    releaseMs  = 30;
};


/*
        A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
    */
template < typename SampleType >
class ReorderableDeEsser : public DeEsser< SampleType >,
                           public ReorderableEffect< SampleType >
{
protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) final;

    void fxChain_prepare (double samplerate, int blocksize) final;
};

}  // namespace bav::dsp::FX
