
/** @ingroup misc
 *  @{
 */
#pragma once

/*
 Saturator, based on a piecewise transfer function originally created by Evan Berard.
 */

namespace lemons::dsp::FX
{
template < typename SampleType >
class Saturator : AudioEffect< SampleType >
{
public:
    void setHardness (float newHardness);

private:
    void serialize (TreeReflector& ref) final;

    void process (juce::AudioBuffer< SampleType >& audio) final;

    SampleType processSample (SampleType inputSample);

    SampleType transferFunc (SampleType amplitude);

    void prepare (double samplerate, int blocksize) final;
    void bypassedBlock (int numSamples) final;

    SampleType hardness {1.};
};

}  // namespace lemons::dsp::FX

/** @}*/
