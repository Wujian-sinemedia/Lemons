#pragma once

#include <lemons_core/lemons_core.h>

namespace lemons::dsp::FX
{
/**
	A class that wraps an AudioEffect into a dsp::Engine.
	@tparam EffectType A template class representing the type of effect this engine will manage -- for example, EQ<SampleType> or Compressor<SampleType>.
	@tparam SampleType The specialization of EffectType that this engine will instantiate. Must be float or double.
	@see AudioEffect, dsp::Engine
 */
template <template <typename FloatType> class EffectType, typename SampleType, LEMONS_MUST_INHERIT_FROM (EffectType<SampleType>, AudioEffect<SampleType>)>
class EffectEngine : public dsp::Engine<SampleType>
{
public:
	/** Creates an EffectEngine.
		Forwards all constructor arguments to the underlying effect object.
	*/
	template <typename... Args>
	EffectEngine (Args&&... args)
		: effect (std::forward<Args> (args)...)
	{
	}

	/** Returns a reference to the underlying effect object. */
	EffectType<SampleType>& operator()() { return effect; }

	/** Returns a reference to the underlying effect object. */
	EffectType<SampleType>& operator*() { return effect; }

	/** Returns a pointer to the underlying effect object. */
	EffectType<SampleType>* operator->() { return &effect; }

private:
	void renderBlock (const AudioBuffer<SampleType>& input,
		AudioBuffer<SampleType>&					 output,
		MidiBuffer&,
		bool isBypassed) final
	{
		if (isBypassed)
		{
			output.clear();
			effect.bypassedBlock (output.getNumSamples());
			return;
		}

		buffers::copy (input, storage);

		effect.process (storage);

		buffers::copy (storage, output);
	}

	void prepared (int blocksize, double samplerate, int) final
	{
		effect.prepare (samplerate, blocksize);
		storage.setSize (2, blocksize);
	}

	void released() final
	{
		storage.setSize (0, 0);
	}

	EffectType<SampleType>	effect;
	AudioBuffer<SampleType> storage;
};

}  // namespace lemons::dsp::FX
