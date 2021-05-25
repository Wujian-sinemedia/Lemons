
#pragma once


namespace bav::dsp::FX
{
template < typename SampleType >
class Delay
{
public:
    Delay() { }

    virtual ~Delay() = default;

    void setDelay (int delayInSamples)
    {
        delay.setDelay (SampleType (delayInSamples));
    }

    void prepare (int blocksize, double samplerate, int numChannels)
    {
        jassert (samplerate > 0);

        spec.sampleRate       = samplerate;
        spec.maximumBlockSize = juce::uint32 (blocksize);
        spec.numChannels      = juce::uint32 (numChannels);

        delay.prepare (spec);

        dryGain.reset (blocksize);
        wetGain.reset (blocksize);
    }

    void reset()
    {
        delay.reset();
        const auto blocksize = int (spec.maximumBlockSize);
        dryGain.reset (blocksize);
        wetGain.reset (blocksize);
    }

    void setDryWet (int wetMixPercent)
    {
        const auto wet = wetMixPercent * 0.01f;
        wetGain.setTargetValue (smoothingZeroCheck (wet));
        dryGain.setTargetValue (smoothingZeroCheck (1.0f - wet));
    }


    void pushSample (int channel, SampleType sample)
    {
        delay.pushSample (channel, sample);
    }


    SampleType popSample (int channel, SampleType* delayLevel = nullptr)
    {
        const auto delaySamps = delay.getDelay();

        const auto drySample = delay.popSample (channel, SampleType (0), false)
                             * dryGain.getNextValue();
        const auto wetSample =
            delay.popSample (channel, delaySamps, true) * wetGain.getNextValue();

        if (delayLevel != nullptr) *delayLevel = std::abs (wetSample);

        return drySample + wetSample;
    }


    void process (int         channelNum,
                  SampleType* samples,
                  int         numSamples,
                  SampleType* delayLevel = nullptr)
    {
        jassert (numSamples > 0);

        for (int i = 0; i < numSamples; ++i)
            pushSample (channelNum, samples[i]);

        auto avgMag = SampleType (0.0);

        for (int i = 0; i < numSamples; ++i)
        {
            SampleType mag;
            *(samples + i) = popSample (channelNum, &mag);
            avgMag += mag;
        }

        if (delayLevel != nullptr)
        {
            avgMag /= numSamples;
            *delayLevel = avgMag;
        }
    }


    void process (juce::AudioBuffer< SampleType >& audio,
                  SampleType*                      delayLevel = nullptr)
    {
        const auto numSamples  = audio.getNumSamples();
        const auto numChannels = audio.getNumChannels();

        jassert (numChannels > 0);

        auto avgMag = SampleType (0.0);

        for (int i = 0; i < numChannels; ++i)
        {
            SampleType mag;
            process (i, audio.getWritePointer (i), numSamples, &mag);
            avgMag += mag;
        }

        if (delayLevel != nullptr)
        {
            avgMag /= numChannels;
            *delayLevel = avgMag;
        }
    }


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


template class Delay< float >;
template class Delay< double >;


/*
    A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
*/

template < typename SampleType >
class ReorderableDelay : public Delay< SampleType >,
                         public ReorderableEffect< SampleType >
{
public:
    ReorderableDelay() { }

protected:
    void fxChain_process (juce::AudioBuffer< SampleType >& audio) override
    {
        Delay< SampleType >::process (audio);
    }

    void fxChain_prepare (double samplerate, int blocksize) override
    {
        Delay< SampleType >::prepare (blocksize, samplerate, 2);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReorderableDelay)
};

template class ReorderableDelay< float >;
template class ReorderableDelay< double >;


}  // namespace bav::dsp::FX
