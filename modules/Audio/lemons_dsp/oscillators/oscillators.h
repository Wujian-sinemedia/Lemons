#pragma once

namespace lemons::dsp::osc
{
/** Represents the phase of an oscillator.
    @see Oscillator
 */
template <typename SampleType>
struct Phase
{
	/** Resets the phase, */
	void resetPhase() noexcept;

	/** Sets the output frequency of the oscillator and its samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate);

	/** Returns the current phase increment. */
	SampleType getIncrement() const noexcept;

	/** Returns the next phase value and handles wraparound logic. */
	SampleType next (SampleType wrapLimit) noexcept;

private:
	SampleType phase = 0, increment = 0;
};

/*--------------------------------------------------------------------------------------------*/

/** Base class for any kind of oscillator.
    Oscillators process only a single channel of samples at a time.
    @see Phase
 */
template <typename SampleType>
struct Oscillator
{
	/** Destructor. */
	virtual ~Oscillator() = default;

	/** This should reset the oscillator's phase. */
	virtual void resetPhase() = 0;

	/** This should set the oscillator's output frequency. */
	virtual void setFrequency (SampleType frequency, SampleType sampleRate) = 0;

	/** This should return the next output sample for the oscillator. */
	virtual SampleType getSample() = 0;

	/** Returns a stream of samples from the oscillator. */
	void getSamples (SampleType* output, int numSamples);
    
    /** Returns a stream of samples from the oscillator. */
    void getSamples (juce::AudioBuffer<SampleType>& output, int channel = 0);
};

/*--------------------------------------------------------------------------------------------*/

/** A basic sine wave oscillator.
    @see Oscillator, Phase
 */
template <typename SampleType>
struct Sine : public Oscillator<SampleType>
{
	/** Constructs a default sine oscillator. */
	Sine();

	/** Resets the sine wave's phase. */
	void resetPhase() final;

	/** Sets the output frequency and samplerate of the sine wave. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the next sample of the sine wave. */
	SampleType getSample() final;

private:
	Phase<SampleType>     phase;
	static constexpr auto twoPi = static_cast<SampleType> (3.141592653589793238 * 2.0);
};

/*--------------------------------------------------------------------------------------------*/

/** A basic sawtooth wave oscillator.
    @see SuperSaw, Oscillator, Phase
 */
template <typename SampleType>
struct Saw : public Oscillator<SampleType>
{
	/** Constructs a default sawtooth oscillator. */
	Saw();

	/** Resets the sawtooth wave's phase. */
	void resetPhase();

	/** Sets the output frequency and samplerate of the sawtooth wave. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the next sample of the sawtooth wave. */
	SampleType getSample() final;

private:
	Phase<SampleType> phase;
};

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
struct Triangle;


/** A basic square wave oscillator.
    @see Oscillator, Phase
 */
template <typename SampleType>
struct Square : public Oscillator<SampleType>
{
	/** Consructs a default square wave oscillator. */
	Square();

	/** Resets the square wave's phase. */
	void resetPhase() final;

	/** Sets the square wave's output frequency and samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the next sample of the square wave. */
	SampleType getSample() final;

private:
	friend struct Triangle<SampleType>;

	Phase<SampleType> phase;
};

/*--------------------------------------------------------------------------------------------*/

/** A basic triangle wave oscillator.
    @see Square, Oscillator, Phase
 */
template <typename SampleType>
struct Triangle : public Oscillator<SampleType>
{
	/** Constructs a default triangle wave. */
	Triangle();

	/** Resets the triangle wave's phase. */
	void resetPhase() final;

	/** Sets the triangle wave's output frequency and samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the next sample of the triangle wave. */
	SampleType getSample() final;

private:
	Square<SampleType> square;
	SampleType         sum = 1;
};

}  // namespace lemons::dsp::osc
