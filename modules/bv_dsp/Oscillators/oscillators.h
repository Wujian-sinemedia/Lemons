
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

    // Returns the current phase before incrementing (and wrapping) it.
    SampleType next (SampleType wrapLimit) noexcept
    {
        const auto p = phase;
        
        phase += increment;
        while (phase >= wrapLimit) phase -= wrapLimit;
        
        return p;
    }
    
    SampleType getIncrement() const { return increment; }

private:
    SampleType phase = 0, increment = 0;
};

/*--------------------------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Oscillator
{
    virtual ~Oscillator() = default;
    
    virtual void resetPhase() = 0;
    virtual void setFrequency (SampleType frequency, SampleType sampleRate) = 0;
    virtual SampleType getSample() = 0;
    
    void getSamples (SampleType* output, const int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            output[i] = getSample();
    }
};

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Sine  :   public Oscillator<SampleType>
{
    Sine() { phase.resetPhase(); }
    virtual ~Sine() = default;

    void resetPhase() final { phase.resetPhase(); }

    void setFrequency (SampleType frequency, SampleType sampleRate) final
    {
        phase.setFrequency (twoPi * frequency, sampleRate);
    }

    SampleType getSample() final { return std::sin (phase.next (twoPi)); }

private:
    Phase< SampleType >   phase;
    static constexpr auto twoPi = static_cast< SampleType > (3.141592653589793238 * 2.0);
};

template struct Sine<float>;
template struct Sine<double>;

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Saw   :   public Oscillator<SampleType>
{
    Saw() { phase.resetPhase(); }
    virtual ~Saw() = default;

    void resetPhase() final { phase.resetPhase(); }

    void setFrequency (SampleType frequency, SampleType sampleRate) final
    {
        phase.setFrequency (frequency, sampleRate);
    }

    SampleType getSample() final
    {
        auto p = phase.next (1);
        return SampleType (2.0) * p - SampleType (1.0) - blep (p, phase.getIncrement());
    }

private:
    Phase< SampleType > phase;
};

template struct Saw<float>;
template struct Saw<double>;

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Square  :     public Oscillator<SampleType>
{
    Square() { phase.resetPhase(); }
    virtual ~Square() = default;

    void resetPhase() final { phase.resetPhase(); }

    void setFrequency (SampleType frequency, SampleType sampleRate) final
    {
        phase.setFrequency (frequency, sampleRate);
    }

    SampleType getSample() final
    {
        const auto inc = phase.getIncrement();
        const auto p   = phase.next (1);

        return (p < SampleType (0.5) ? SampleType (-1) : SampleType (1))
             - blep (p, inc)
             + blep (std::fmod (p + SampleType (0.5), SampleType (1)), inc);
    }
    
    SampleType getIncrement() const { return phase.getIncrement(); }

private:
    Phase< SampleType > phase;
};

template struct Square<float>;
template struct Square<double>;

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Triangle  :   public Oscillator<SampleType>
{
    Triangle() { resetPhase(); }
    virtual ~Triangle() = default;

    void resetPhase() final
    {
        square.resetPhase();
        sum = static_cast< SampleType > (1);
    }

    void setFrequency (SampleType frequency, SampleType sampleRate) final
    {
        square.setFrequency (frequency, sampleRate);
    }

    SampleType getSample() final
    {
        sum += SampleType (4) * square.getIncrement() * square.getSample();
        return sum;
    }

private:
    Square< SampleType > square;
    SampleType           sum = 1;
};

template struct Triangle<float>;
template struct Triangle<double>;


}  // namespace bav::dsp::osc
