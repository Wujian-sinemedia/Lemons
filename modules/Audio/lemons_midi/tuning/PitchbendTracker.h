
#pragma once

namespace lemons::midi
{

/** @ingroup lemons_midi_tuning
 */
class PitchBendTracker
{
public:
	void setRange (int newStUp, int newStDown) noexcept;

	int getRangeUp() const noexcept;
	int getRangeDown() const noexcept;

	int getLastReceivedPitchbend() const noexcept;

	void newPitchbendReceived (int newPitchbend) noexcept;

	void processMidiMessage (const MidiMessage& m) noexcept;

	template <typename NoteType>
	float getAdjustedMidiPitch (NoteType midiPitch) const
	{
		if (lastReceivedPitchbend == 64.f) return static_cast<float> (midiPitch);

		if (lastReceivedPitchbend > 64.f)
			return static_cast<float> (((rangeUp * (lastReceivedPitchbend - 65.f)) / 62.f)
			                           + float (midiPitch));

		return static_cast<float> ((((1.f - rangeDown) * lastReceivedPitchbend) / 63.f) + float (midiPitch)
		                           - rangeDown);
	}

private:
	float rangeUp { 2.f };
	float rangeDown { 2.f };
	float lastReceivedPitchbend { 64.f };
};

}  // namespace lemons::midi
