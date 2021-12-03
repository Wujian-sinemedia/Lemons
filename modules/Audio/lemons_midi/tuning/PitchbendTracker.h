
#pragma once

namespace lemons::midi
{
// helper class for outputting midifloat values based on input midiPitch, pitchbend, and pitchbend range controls
class PitchBendTracker
{
public:
	void setRange (int newStUp, int newStDown) noexcept;

	int getRangeUp() const noexcept;
	int getRangeDown() const noexcept;

	int getLastRecievedPitchbend() const noexcept;

	void newPitchbendRecieved (int newPitchbend) noexcept;

	void processMidiMessage (const MidiMessage& m) noexcept;

	template <typename NoteType>
	float getAdjustedMidiPitch (NoteType midiPitch) const
	{
		if (lastRecievedPitchbend == 64) return static_cast<float> (midiPitch);

		if (lastRecievedPitchbend > 64)
			return static_cast<float> (((rangeUp * (lastRecievedPitchbend - 65.f)) / 62.f)
			                           + float (midiPitch));

		return static_cast<float> ((((1.f - rangeDown) * lastRecievedPitchbend) / 63.f) + float (midiPitch)
		                           - rangeDown);
	}

private:
	float rangeUp { 2.f };
	float rangeDown { 2.f };
	float lastRecievedPitchbend { 64.f };
};

}  // namespace lemons::midi


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

struct PitchbendTests : public MidiTest
{
    PitchbendTests();
    
private:
    void runTest() final;
};

static PitchbendTests pitchbendTest;

}

#endif
