
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class MonoToStereoPanner : public PannerBase
{
public:
    MonoToStereoPanner() { }

    virtual ~MonoToStereoPanner() = default;


    void prepare (int blocksize)
    {
        left.prepare (blocksize);
        right.prepare (blocksize);
    }

    void reset()
    {
        left.reset();
        right.reset();
    }


    void process (const SampleType* input,
                  SampleType*       leftOut,
                  SampleType*       rightOut,
                  int               numSamples)
    {
        left.setGain (PannerBase::getLeftGain());
        right.setGain (PannerBase::getRightGain());

        vecops::copy (input, leftOut, numSamples);
        vecops::copy (input, rightOut, numSamples);

        left.process (leftOut, numSamples, 0);
        right.process (rightOut, numSamples, 0);
    }


    void process (const juce::AudioBuffer< SampleType >& monoInput,
                  juce::AudioBuffer< SampleType >&       stereoOutput)
    {
        stereoOutput.clear();
        jassert (stereoOutput.getNumChannels() >= 2);
        jassert (monoInput.getNumSamples() == stereoOutput.getNumSamples());

        process (monoInput.getReadPointer (0),
                 stereoOutput.getWritePointer (0),
                 stereoOutput.getWritePointer (1),
                 stereoOutput.getNumSamples());
    }


private:
    SmoothedGain< SampleType, 1 > left, right;
};


template class MonoToStereoPanner< float >;
template class MonoToStereoPanner< double >;

}  // namespace bav::dsp::FX
