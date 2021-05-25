
#pragma once

namespace bav::dsp::osc
{
template < typename SampleType >
struct Phase
{
    void resetPhase() noexcept { phase = 0; }

    void setFrequency (SampleType frequency, SampleType sampleRate)
    {
        jassert (sampleRate > 0 && frequency > 0);
        increment = frequency / sampleRate;
    }

    /// Returns the current phase before incrementing (and wrapping) it.
    SampleType next (SampleType wrapLimit) noexcept
    {
        auto p = phase;
        phase += increment;

        while (phase >= wrapLimit)
            phase -= wrapLimit;

        return p;
    }

    SampleType phase = 0, increment = 0;
};


template < typename SampleType >
static SampleType blep (SampleType phase, SampleType increment) noexcept
{
    static constexpr SampleType one = 1;

    if (phase < increment)
    {
        auto p = phase / increment;
        return (2 - p) * p - one;
    }

    if (phase > one - increment)
    {
        auto p = (phase - one) / increment;
        return (p + 2) * p + one;
    }

    return SampleType (0);
}

/*--------------------------------------------------------------------------------------------
    ---------------------------------------------------------------------------------------------*/

template < typename SampleType >
class Sine
{
public:
    Sine() { phase.resetPhase(); }
    virtual ~Sine() = default;

    void resetPhase() noexcept { phase.resetPhase(); }

    void setFrequency (SampleType frequency, SampleType sampleRate)
    {
        phase.setFrequency (twoPi * frequency, sampleRate);
    }

    SampleType getSample() noexcept { return std::sin (phase.next (twoPi)); }

    void getSamples (SampleType* BV_R_ output, const int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            output[i] = getSample();
    }

private:
    Phase< SampleType >   phase;
    static constexpr auto twoPi =
        static_cast< SampleType > (3.141592653589793238 * 2.0);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sine)
};

/*
    */

template < typename SampleType >
class Saw
{
public:
    Saw() { phase.resetPhase(); }
    virtual ~Saw() = default;

    void resetPhase() noexcept { phase.resetPhase(); }

    void setFrequency (SampleType frequency, SampleType sampleRate)
    {
        phase.setFrequency (frequency, sampleRate);
    }

    SampleType getSample() noexcept
    {
        auto p = phase.next (1);
        return SampleType (2.0) * p - SampleType (1.0) - blep (p, phase.increment);
    }

    void getSamples (SampleType* BV_R_ output, const int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            output[i] = getSample();
    }

private:
    Phase< SampleType > phase;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Saw)
};

/*
    */

template < typename SampleType >
class Square
{
public:
    Square() { phase.resetPhase(); }
    virtual ~Square() = default;

    void resetPhase() noexcept { phase.resetPhase(); }

    void setFrequency (SampleType frequency, SampleType sampleRate)
    {
        phase.setFrequency (frequency, sampleRate);
    }

    SampleType getSample() noexcept
    {
        auto                  p    = phase.next (1);
        static constexpr auto half = SampleType (0.5);

        return (p < half ? SampleType (-1) : SampleType (1))
             - blep (p, phase.increment)
             + blep (std::fmod (p + half, SampleType (1)), phase.increment);
    }

    void getSamples (SampleType* BV_R_ output, const int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            output[i] = getSample();
    }

private:
    Phase< SampleType > phase;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Square)
};

/*
    */

template < typename SampleType >
class Triangle
{
public:
    Triangle() { resetPhase(); }
    virtual ~Triangle() = default;

    void resetPhase() noexcept
    {
        square.resetPhase();
        sum = static_cast< SampleType > (1);
    }

    void setFrequency (SampleType frequency, SampleType sampleRate)
    {
        square.setFrequency (frequency, sampleRate);
    }

    SampleType getSample() noexcept
    {
        sum += SampleType (4) * square.phase.increment * square.getSample();
        return sum;
    }

    void getSamples (SampleType* BV_R_ output, const int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            output[i] = getSample();
    }

private:
    Square< SampleType > square;
    SampleType           sum = 1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Triangle)
};


}  // namespace bav::dsp::osc
