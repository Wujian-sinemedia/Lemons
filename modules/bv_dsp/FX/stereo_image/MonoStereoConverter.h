
namespace bav::dsp::FX
{
template < typename SampleType >
class MonoStereoConverter
{
    using Buffer = juce::AudioBuffer< SampleType >;

public:
    MonoStereoConverter()
        : toMonoMode (leftOnly)
        , monoStorage (1, 0)
    {
    }

    virtual ~MonoStereoConverter() = default;


    enum StereoReductionMode
    {
        leftOnly,
        rightOnly,
        mixToMono
    };


    void prepare (int blocksize)
    {
        monoStorage.setSize (1, blocksize, true, true, true);
    }


    void setStereoReductionMode (StereoReductionMode newmode)
    {
        toMonoMode = newmode;
    }

    StereoReductionMode getStereoReductionMode() const { return toMonoMode; }


    void convertStereoToMono (const SampleType* leftIn,
                              const SampleType* rightIn,
                              SampleType*       monoOut,
                              int               numSamples)
    {
        switch (toMonoMode)
        {
            case (leftOnly):
            {
                vecops::copy (leftIn, monoOut, numSamples);
            }
            case (rightOnly):
            {
                vecops::copy (rightIn, monoOut, numSamples);
            }
            case (mixToMono):
            {
                const auto pnt5 = static_cast< SampleType > (0.5);
                monoStorage.copyFrom (0, 0, leftIn, numSamples, pnt5);
                monoStorage.addFrom (0, 0, rightIn, numSamples, pnt5);
                vecops::copy (monoStorage.getReadPointer (0), monoOut, numSamples);
            }
        }
    }

    void convertStereoToMono (const Buffer& input, Buffer& output)
    {
        jassert (input.getNumSamples() == output.getNumSamples());

        if (input.getNumChannels() == 1)
        {
            vecops::copy (input.getReadPointer (0),
                          output.getWritePointer (0),
                          output.getNumSamples());
        }
        else
        {
            convertStereoToMono (input.getReadPointer (0),
                                 input.getReadPointer (1),
                                 output.getWritePointer (0),
                                 output.getNumSamples());
        }
    }


    void convertMonoToStereo (const SampleType* monoIn,
                              SampleType*       leftOut,
                              SampleType*       rightOut,
                              int               numSamples)
    {
        vecops::copy (monoIn, leftOut, numSamples);
        vecops::copy (monoIn, rightOut, numSamples);
    }

    void convertMonoToStereo (const Buffer& input, Buffer& output)
    {
        jassert (input.getNumSamples() == output.getNumSamples());
        jassert (output.getNumChannels() >= 2);

        convertMonoToStereo (input.getReadPointer (0),
                             output.getWritePointer (0),
                             output.getWritePointer (1),
                             output.getNumSamples());
    }


private:
    StereoReductionMode toMonoMode;

    juce::AudioBuffer< SampleType > monoStorage;
};


template class MonoStereoConverter< float >;
template class MonoStereoConverter< double >;


} // namespace bav::dsp::FX
