#pragma once


namespace lemons::midi
{

class MidiKeyboardState
{
public:
	MidiKeyboardState();

	bool isNoteOn (int note) const;

	void noteOn (int note);
	void noteOff (int note);

	void processMidiMessage (const MidiMessage& m);

	void setFromArrayOfPitches (const juce::Array<int>& pitches);

	juce::Array<int> getAsArrayOfPitches() const;

	void allNotesOff();

private:
	std::array<bool, 128> notes;
};

}  // namespace lemons::midi
