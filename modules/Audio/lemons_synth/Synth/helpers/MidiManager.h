#pragma once

#include <lemons_midi/lemons_midi.h>

namespace lemons::dsp::synth
{

using juce::MidiMessage;

template <typename SampleType>
class MidiManager : public midi::ChoppingProcessor<SampleType>
{
public:
	MidiManager (SynthBase<SampleType>& s)
	    : router (s)
	    , synth (s)
	{
	}

	/*---------------*/

	class MidiRouter : public midi::MidiProcessor
	{
	public:
		MidiRouter (SynthBase<SampleType>& s)
		    : synth (s)
		{
		}

	private:
		void handleNoteOn (int midiPitch, float velocity) final;
		void handleNoteOff (int midiPitch, float velocity) final;
		void handlePitchwheel (int wheelValue) final;
		void handleAftertouch (int noteNumber, int aftertouchValue) final;
		void handleChannelPressure (int channelPressureValue) final;
		void handleSustainPedal (int controllerValue) final;
		void handleSostenutoPedal (int controllerValue) final;
		void handleSoftPedal (int controllerValue) final;
		void handleAllSoundOff() final { synth.allNotesOff (false); }

		SynthBase<SampleType>& synth;
	};

	/*---------------*/

	MidiRouter router;

private:
	void handleMidiMessage (const MidiMessage& m) final;
	void renderChunk (AudioBuffer<SampleType>& audio, MidiBuffer&) final;

	SynthBase<SampleType>& synth;
};

}  // namespace lemons::dsp::synth
