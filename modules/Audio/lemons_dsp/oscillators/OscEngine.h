#pragma once

namespace lemons::dsp::osc
{
/** A utility class that wraps an Oscillator into a self-contained dsp::Engine.
    This will instantiate and manage the appropriate oscillator type for you. \n
    Example usage:
    @code
    using namspace lemons::dsp::osc;

    auto sineEngine = Engine<float, Sine>;
    @endcode
    @tparam SampleType The floating point type to be used for samples. Must be either float or double.
    @tparam Osctype A template class representing the kind of oscillator you want this engine to use; for example, osc::Sine. This should be un-specialized. This type must inherit from Oscillator.
    @see Oscillator, dsp::Engine
 */
template <typename SampleType, template <typename T> class Osctype>
class Engine : public dsp::Engine<SampleType>
{
public:
	/** Updates the frequency of the owned oscillator object. */
	void setFrequency (float freqHz)
	{
		jassert (freqHz > 0.f);

		frequency = freqHz;

		const auto sr = static_cast<SampleType> (this->getSamplerate());

		for (auto* osc : oscillators)
			osc->setFrequency (frequency, sr);
	}

	/** Returns the oscillator's current frequency. */
	float getFrequency() const noexcept { return frequency; }

private:
	using Osc = Osctype<SampleType>;

	void renderBlock (const AudioBuffer<SampleType>&,
	                  AudioBuffer<SampleType>& output,
	                  MidiBuffer&, bool isBypassed) final
	{
		const auto numSamples  = output.getNumSamples();
		const auto numChannels = output.getNumChannels();

		jassert (oscillators.size() == numChannels);

		if (isBypassed)
		{
			for (auto* osc : oscillators)
				osc->skipSamples (numSamples);
		}
		else
		{
			for (int chan = 0; chan < numChannels; ++chan)
				oscillators.getUnchecked (chan)->getSamples (output, chan);
		}
	}

	void prepared (int, double samplerate, int numChannels) final
	{
		while (oscillators.size() < numChannels)
			oscillators.add (new Osc());

		while (oscillators.size() > numChannels)
			oscillators.removeLast();

		for (auto* osc : oscillators)
		{
			osc->resetPhase();
			osc->setFrequency (frequency,
			                   static_cast<SampleType> (samplerate));
		}
	}

	juce::OwnedArray<Osc> oscillators;

	float frequency { 440.f };
};

}  // namespace lemons::dsp::osc
