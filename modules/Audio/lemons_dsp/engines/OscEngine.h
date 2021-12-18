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
template <typename SampleType, template <typename T> class Osctype,
          LEMONS_MUST_INHERIT_FROM (Osctype<SampleType>, Oscillator<SampleType>)>
class Engine final : public dsp::Engine<SampleType>
{
public:
	/** Updates the frequency of the owned oscillator object. */
	void setFrequency (float freqHz)
	{
		jassert (freqHz > 0.f);

		frequency = freqHz;

		const auto sr = static_cast<SampleType> (this->getSamplerate());
        const auto freq = static_cast<SampleType> (frequency);

		for (auto* osc : oscillators)
			osc->setFrequency (freq, sr);
	}

	/** Returns the oscillator's current frequency. */
	[[nodiscard]] float getFrequency() const noexcept { return frequency; }

private:
	void renderBlock (const AudioBuffer<SampleType>&,
	                  AudioBuffer<SampleType>& output,
	                  MidiBuffer&, bool isBypassed) final
	{
		const auto numSamples  = output.getNumSamples();
		const auto numChannels = output.getNumChannels();

		jassert (oscillators->size() == numChannels);
        
        output.clear();

		if (isBypassed)
		{
			for (auto* osc : oscillators)
				osc->skipSamples (numSamples);
		}
		else
		{
			for (int chan = 0; chan < numChannels; ++chan)
				oscillators[chan]->getSamples (output, chan);
		}
	}

	void prepared (int, double samplerate, int numChannels) final
	{
		oscillators.resize (numChannels);

		for (auto* osc : oscillators)
		{
			osc->resetPhase();
			osc->setFrequency (frequency, static_cast<SampleType> (samplerate));
		}
	}

	void released() final
	{
		for (auto* osc : oscillators)
			osc->resetPhase();
	}

	ConstructedArray<Osctype<SampleType>> oscillators;

	float frequency { 440.f };
};

}  // namespace lemons::dsp::osc
