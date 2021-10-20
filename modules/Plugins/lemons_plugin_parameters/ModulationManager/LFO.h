#pragma once

#include <lemons_oscillators/lemons_oscillators.h>


namespace lemons::plugin
{

/** An LFO, to be used by a ModulationManager object.
 Internally, this class uses a dsp::osc::ChoosableOscillator to generate its values.
 */
struct LFO : SerializableData
{
	/** Internally, LFOs use ChoosableOscillator to generate their values. */
	using Osc = dsp::osc::ChoosableOscillator<float>;

	/** Creates a new LFO. */
	LFO (ParameterList& listToUse, String metaParameterName = "MetaParameter");

	/** Prepares the LFO for processing. */
	void prepareToPlay (int numSamples, double samplerate);

	/** Returns a reference to the LFO's internal ChoosableOscillator object. You can then call methods on the oscillator to, for example, set its frequency, change the type, etc. */
	[[nodiscard]] Osc& getOscillator() noexcept;

	[[nodiscard]] DefaultMetaParameter& getParameter() noexcept;

	void addParameterTo (juce::AudioProcessor& processor);

private:
	void serialize (TreeReflector& ref) final;

	Osc osc;

	MetaParam param;

	AudioBuffer<float> storage;
};

}  // namespace lemons::plugin
