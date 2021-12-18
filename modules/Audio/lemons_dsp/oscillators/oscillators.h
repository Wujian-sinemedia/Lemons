/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#pragma once

namespace lemons::dsp::osc
{
/** Represents the phase of an oscillator.
    @see Oscillator
 */
template <typename SampleType>
struct Phase final
{
	/** Resets the phase, */
	void resetPhase() noexcept;

	/** Sets the output frequency and samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate);

	/** Returns the current phase increment. */
	[[nodiscard]] SampleType getIncrement() const noexcept;

	/** Returns the next phase value and handles wraparound logic. */
	[[nodiscard]] SampleType next (SampleType wrapLimit) noexcept;

private:
	SampleType phase { 0 }, increment { 0 };
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

	/** Resets the oscillator's phase. */
	virtual void resetPhase() = 0;

	/** Sets the oscillator's output frequency and samplerate. */
	virtual void setFrequency (SampleType frequency, SampleType sampleRate) = 0;

	/** Returns the oscillator's frequency. */
	[[nodiscard]] virtual SampleType getFrequency() const noexcept = 0;

	/** Returns the next output sample for the oscillator. */
	[[nodiscard]] SampleType getSample();

	/** Returns a stream of samples from the oscillator. */
	void getSamples (SampleType* output, int numSamples);

	/** Returns a stream of samples from the oscillator. */
	void getSamples (AudioBuffer<SampleType>& output, int channel = 0);

	/** Skips a number of samples in the stream. */
	void skipSamples (int numToSkip);

protected:
	/** Creates an oscillator with a specified lambda function for producing the next sample. */
	explicit Oscillator (std::function<SampleType()>&& sampleFuncToUse);

private:
	std::function<SampleType()> sampleFunc;
};

/*--------------------------------------------------------------------------------------------*/

/** A basic sine wave oscillator.
    @see Oscillator, Phase
 */
template <typename SampleType>
struct Sine final : public Oscillator<SampleType>
{
	/** Constructs a default sine oscillator. */
	explicit Sine();

	/** Resets the sine wave's phase. */
	void resetPhase() final;

	/** Sets the output frequency and samplerate of the sine wave. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the sine wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:
	Phase<SampleType> phase;
	SampleType        freq { 0 };
};

/*--------------------------------------------------------------------------------------------*/

/** A basic sawtooth wave oscillator.
    @see SuperSaw, Oscillator, Phase
 */
template <typename SampleType>
struct Saw final : public Oscillator<SampleType>
{
	/** Constructs a default sawtooth oscillator. */
	explicit Saw();

	/** Resets the sawtooth wave's phase. */
	void resetPhase();

	/** Sets the output frequency and samplerate of the sawtooth wave. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the sawtooth wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:
	Phase<SampleType> phase;
	SampleType        freq { 0 };
};

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
struct Triangle;


/** A basic square wave oscillator.
    @see Oscillator, Phase
 */
template <typename SampleType>
struct Square final : public Oscillator<SampleType>
{
	/** Consructs a default square wave oscillator. */
	explicit Square();

	/** Resets the square wave's phase. */
	void resetPhase() final;

	/** Sets the square wave's output frequency and samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the square wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:
	friend struct Triangle<SampleType>;

	Phase<SampleType> phase;
	SampleType        freq { 0 };
};

/*--------------------------------------------------------------------------------------------*/

/** A basic triangle wave oscillator.
    @see Square, Oscillator, Phase
 */
template <typename SampleType>
struct Triangle final : public Oscillator<SampleType>
{
	/** Constructs a default triangle wave. */
	explicit Triangle();

	/** Resets the triangle wave's phase. */
	void resetPhase() final;

	/** Sets the triangle wave's output frequency and samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the square wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:
	Square<SampleType> square;
	SampleType         sum { 1 };
	SampleType         freq { 0 };
};

}  // namespace lemons::dsp::osc


/*--------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename SampleType>
struct OscillatorTests : public DspTest
{
	OscillatorTests();

private:
	void runTest() final;

	void runOscillatorTests (dsp::osc::Oscillator<SampleType>& osc);

	dsp::osc::Sine<SampleType>     sine;
	dsp::osc::Saw<SampleType>      saw;
	dsp::osc::Square<SampleType>   square;
	dsp::osc::Triangle<SampleType> triangle;

	AudioBuffer<SampleType> storage;

	juce::Array<int> zeroCrossings;
};

LEMONS_CREATE_DSP_TEST (OscillatorTests)

}  // namespace lemons::tests

#endif
