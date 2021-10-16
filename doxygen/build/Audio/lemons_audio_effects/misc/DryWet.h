
/** @ingroup misc
 *  @{
 */
#pragma once

namespace lemons::dsp::FX
{
/**
    A basic dry/wet mixer class for combining and mixing together two streams of audio.
    This class doesn't inherit from AudioEffect, because its API had to be slightly different, but I also figure that its use cases are slightly different than those of the effect classes. \n
    This class is essentially a wrapper around juce::dsp::DryWetMixer.
    @see SmoothedGain
 */
template < typename SampleType >
class DryWetMixer
{
public:
    /** Creates a DryWetMixer with some default inital settings. */
    DryWetMixer();

    /** Creates a DryWetMixer with some specified initial settings. */
    DryWetMixer (int wetPercentMix, int wetLatencySamples = 0);

    /** Sets the amount of wet signal that will be present in the mixed output.
        For example, if you pass 100, the output signal will be entirely wet and no dry. If you pass 0, the output signal will be entirely dry and no wet. If you pass 50, the output signal will be 50% dry and 50% wet.
     */
    void setWetMix (int wetPercent);

    /** Sets the latency in samples of the wet signal relative to the dry signal. */
    void setWetLatency (int latencySamples);

    /** Prepares the DryWetMixer. */
    void prepare (int numChannels, int blocksize, double samplerate);

    /** Resets the internal state of the DryWetMixer to a neutral one. */
    void reset();


    /** Pushes dry samples into the DryWetMixer's internal buffers.
        This should be called before calling mixWetSamples. \n \n
        The contents of the passed buffer are not altered, but the buffer itself can't be const due to juce::dsp::AudioBlock's constructor. Take it up with Jules  ¯\_(ツ)_/¯
        @see mixWetSamples(), process()
     */
    void pushDrySamples (AudioBuffer< SampleType >& buffer);


    /** Mixes wet samples with the previously pushed dry samples.
        This should be called after calling pushDrySamples. \n \n
        The mixed output signal will be written back to the passed buffer.
        @see pushDrySamples(), process()
     */
    void mixWetSamples (AudioBuffer< SampleType >& buffer);


    /** Mixes together a stream of dry and wet samples with one function call.
        This is the same as calling pushDrySamples and then mixWetSamples. \n \n
        The contents of the dry buffer are not altered, but the buffer itself can't be const due to juce::dsp::AudioBlock's constructor. \n \n
        The mixed output signal will be written to the wet buffer.
        @see pushDrySamples(), mixWetSamples()
     */
    void process (AudioBuffer< SampleType >& dry, AudioBuffer< SampleType >& wet);

private:
    juce::dsp::DryWetMixer< SampleType > mixer {512};
    juce::dsp::ProcessSpec               spec;
};

}  // namespace lemons::dsp::FX

/** @}*/
