/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons::dsp
{

/** Base class for any kind of oscillator.
	Oscillators process only a single channel of samples at a time.
 */
template <typename SampleType>
struct Oscillator : public SampleStream<SampleType>
{
	/** You should provide the Oscillator constructor with a lambda for producing the next sample, identical to the SampleStream constructor. */
	using SampleStream<SampleType>::SampleStream;

	/** Resets the oscillator's phase. */
	virtual void resetPhase() = 0;

	/** Sets the oscillator's output frequency and samplerate. */
	virtual void setFrequency (SampleType frequency, SampleType sampleRate) = 0;

	/** Returns the oscillator's frequency. */
	[[nodiscard]] virtual SampleType getFrequency() const noexcept = 0;

	/** Represents the phase of an oscillator.
	 */
	struct Phase final
	{
		Phase() = default;

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

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Phase)
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};

}  // namespace lemons::dsp
