
/** @ingroup time
 *  @{
 */

#pragma once


namespace lemons::dsp::FX
{
template < typename SampleType >
class Delay : public LevelReportingAudioEffect< SampleType >
{
public:
    void prepare (double samplerate, int blocksize) final;

    void reset();

    void setDelay (int delayInSamples);
    void setDryWet (int wetMixPercent);

    void       pushSample (int channel, SampleType sample);
    SampleType popSample (int channel, SampleType* delayLevel = nullptr);

private:
    void serialize (TreeReflector& ref) final;

    SampleType processChannel (int         channel,
                               int         numSamples,
                               SampleType* signal,
                               const SampleType*) final;

    juce::dsp::DelayLine< SampleType > delay;
    juce::dsp::ProcessSpec             spec;

    ValueSmoother< SampleType > dryGain, wetGain;

    int wetPcnt {50};
};

}  // namespace lemons::dsp::FX

/** @}*/
