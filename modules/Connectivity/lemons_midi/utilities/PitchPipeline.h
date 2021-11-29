#pragma once

#include <mts_esp_cpp/mts_esp_cpp.h>

namespace lemons::midi
{
struct PitchPipeline
{
	float getFrequencyForMidi (int midiPitch, int midiChannel = -1) const;
	float getFrequencyForMidi (float midiPitch, int midiChannel = -1) const;

	float getMidiForFrequency (int midiPitch, int midiChannel = -1) const;
	float getMidiForFrequency (float midiPitch, int midiChannel = -1) const;

	void reset();

	PitchBendTracker bend;
	MTSESP::Client   tuning;
};

}  // namespace lemons::midi
