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

namespace lemons::dsp::osc
{

template <typename SampleType>
class DetunableBase : public Oscillator<SampleType>
{
public:

	explicit DetunableBase (ConstructedArray<Oscillator<SampleType>>& arrayToUse);

	void resetPhase() final;

	/** Sets the output frequency and samplerate of the super saw.
	The frequency set here will be the center frequency that the detuned voices are spread around.
	*/
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the SuperSaw's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

	/** Controls the total amount of pitch spread between the voices, in cents.
	@param totalPitchSpreadInCents The total pitch spread of all the voices. The 7 voices will be spread evenly around the super saw's center frequency, with the highest and lowest voices being this far apart in cents.
	*/
	void setDetuneAmount (int totalPitchSpreadInCents);

	/** Returns the total pitch spread of the voices in cents. */
	[[nodiscard]] int getPitchSpreadCents() const noexcept;

	void changeNumVoices (int newNumVoices);

	[[nodiscard]] int getNumVoices() const noexcept;

private:

	ConstructedArray<Oscillator<SampleType>>& oscillators;

	int totalSpreadCents { 0 };

	SampleType lastFrequency { 440. }, samplerate { 44100. };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DetunableBase)
};


template <typename SampleType, template <typename T> class OscillatorType,
		  LEMONS_MUST_INHERIT_FROM (OscillatorType<SampleType>, Oscillator<SampleType>)>
class Detunable : public DetunableBase<SampleType>
{
public:

	Detunable()
		: DetunableBase<SampleType> (oscs)
	{
	}

private:

	ConstructedArray<Oscillator<SampleType>> oscs { 1, []()
													{ return new OscillatorType<SampleType>; } };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Detunable)
};

template <typename SampleType>
using SuperSaw = Detunable<SampleType, Saw>;

}  // namespace lemons::dsp::osc
