
#pragma once


namespace bav::dsp::FX
{
template < typename SampleType >
class Delay
{
public:
    Delay()          = default;
    virtual ~Delay() = default;

    void prepare (int blocksize, double samplerate, int numChannels);
    void reset();

    void setDelay (int delayInSamples);
    void setDryWet (int wetMixPercent);

    void       pushSample (int channel, SampleType sample);
    SampleType popSample (int channel, SampleType* delayLevel = nullptr);

    void process (int         channelNum,
                  SampleType* samples,
                  int         numSamples,
                  SampleType* delayLevel = nullptr);

    void process (juce::AudioBuffer< SampleType >& audio,
                  SampleType*                      delayLevel = nullptr);

private:
    template < typename Type >
    inline SampleType smoothingZeroCheck (Type value)
    {
        constexpr auto minSmoothedGain = SampleType (0.0001);
        return std::max (minSmoothedGain, SampleType (value));
    }

    juce::dsp::DelayLine< SampleType > delay;
    juce::dsp::ProcessSpec             spec;

    juce::SmoothedValue< SampleType, juce::ValueSmoothingTypes::Multiplicative >
        dryGain, wetGain;
};


/*
    A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
*/

template < typename SampleType >
class ReorderableDelay : public Delay< SampleType >,
                         public ReorderableEffect< SampleType >
{
protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) final;

    void fxChain_prepare (double samplerate, int blocksize) final;
};

}  // namespace bav::dsp::FX
