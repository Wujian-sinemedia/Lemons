#pragma once

namespace lemons::dsp::osc
{
/** A basic "super saw" oscillator.
    This oscillator contains 7 slightly detuned saw oscillators and returns their aggregate output. \n
    You can set the detuning amount in cents with setDetuneAmount().
    @see Saw, Oscillator, Phase
 */
template <typename SampleType>
class SuperSaw final : public Oscillator<SampleType>
{
public:
	/** Constructs a default SuperSaw. */
	explicit SuperSaw();

	/** Resets the super saw's phase. */
	void resetPhase() final;

	/** Sets the output frequency and samplerate of the super saw.
	    The frequency set here will be the center frequency that the detuned voices are spread around.
	 */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Controls the total amount of pitch spread between the voices, in cents.
	    @param totalPitchSpreadInCents The total pitch spread of all the voices.
	    The 7 voices will be spread evenly around the super saw's center frequency, with the highest and lowest voices being this far apart in cents.
	 */
	void setDetuneAmount (int totalPitchSpreadInCents);

	/** Returns the total pitch spread of the voices in cents. */
	int getPitchSpreadCents() const noexcept;

	/** Returns the next sample of the super saw's output. */
	SampleType getSample() final;

private:
	juce::OwnedArray<Saw<SampleType>> saws;

	int totalSpreadCents { 0 };

	SampleType lastFrequency { 440. }, samplerate { 44100. };
};

}  // namespace lemons::dsp::osc
