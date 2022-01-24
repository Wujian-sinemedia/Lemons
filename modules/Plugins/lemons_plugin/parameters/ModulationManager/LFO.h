#pragma once

#include <lemons_dsp/lemons_dsp.h>


namespace lemons::plugin
{

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;

/** @defgroup parameter_modulation Parameter modulation
	@ingroup lemons_plugin_parameters
	Classes for parameter modulation.
 */

/** @ingroup parameter_modulation
	An LFO, to be used by a ModulationManager object.
	Internally, this class uses a dsp::osc::ChoosableOscillator to generate its values.
 */
struct LFO final : public MetaParameter<float>
{
	using MetaParameter<float>::MetaParameter;

	explicit LFO (const ParameterList& parameterListToUse, const String& paramName);

	/** Internally, LFOs use ChoosableOscillator to generate their values. */
	using Osc = dsp::osc::ChoosableOscillator<float>;

	/** Prepares the LFO for processing. */
	void prepareToPlay (int numSamples, double samplerate);

	void finishBlock (int numSamples);

	[[nodiscard]] float getCurrentOscillatorValue() const noexcept;

	/** Returns a reference to the LFO's internal ChoosableOscillator object. You can then call methods on the oscillator to, for example, set its frequency, change the type, etc. */
	[[nodiscard]] Osc& getOscillator() noexcept;

	[[nodiscard]] ValueTree saveToValueTree() const final;

	void loadFromValueTree (const ValueTree& tree) final;

private:

	Osc osc;

	AudioBuffer<float> storage;

	int readIdx { 0 };
};

}  // namespace lemons::plugin
