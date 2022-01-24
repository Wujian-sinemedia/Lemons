#pragma once

namespace lemons::dsp
{

template <typename SampleType, typename MidiProcessorType, LEMONS_MUST_INHERIT_FROM (MidiProcessorType, midi::Processor)>
struct MidiProcessorEngine : public Engine<SampleType>
{
	template <typename... Args>
	explicit MidiProcessorEngine (Args&&... args)
		: processor (std::forward<Args> (args)...)
	{
	}

	MidiProcessorType processor;

private:
	void renderBlock (const AudioBuffer<SampleType>&, AudioBuffer<SampleType>&, MidiBuffer& midiMessages, bool) final
	{
		processor.process (midiMessages);
	}

	void prepared (int blocksize, double, int) final
	{
		processor.prepare (blocksize);
	}

	void released() final
	{
		processor.release();
	}

	void onReset() final
	{
		processor.reset();
	}
};

}  // namespace lemons::dsp
