#pragma once

namespace lemons::midi
{

/** @defgroup lemons_midi_processors Processors
    @ingroup lemons_midi
    Midi processor base classes.
 */

/** @ingroup lemons_midi_processors
 */
class MidiProcessor
{
public:
	MidiProcessor()          = default;
	virtual ~MidiProcessor() = default;

	void reset();
    
    /** @name midi_processor_processing Processing
     */
    ///@{

	void process (const MidiBuffer& buffer);
	void process (const juce::MidiMessageMetadata& meta);
	void process (const MidiMessage& m);
    
    ///@}
    
    
    /** @name midi_processor_attributes State queries
     */
    ///@{

	int getLastPitchwheelValue() const noexcept { return lastPitchwheelValue; }
	int getLastMidiChannel() const noexcept { return lastMidiChannel; }
	int getLastMidiTimestamp() const noexcept { return lastMidiTimestamp; }

	bool isSustainPedalDown() const noexcept { return sustainPedalDown; }
	bool isSostenutoPedalDown() const noexcept { return sostenutoPedalDown; }
	bool isSoftPedalDown() const noexcept { return softPedalDown; }

	void getLastMovedCCinfo (int& controllerNumber, int& controllerValue) const noexcept;
    
    ///@}

private:
	void processControllerMessage (int controllerNumber, int controllerValue);
	void processPitchWheel (int wheelValue);
	void processSustainPedal (int controllerValue);
	void processSostenutoPedal (int controllerValue);
	void processSoftPedal (int controllerValue);

	virtual void handleNoteOn (int midiPitch, float velocity);
	virtual void handleNoteOff (int midiPitch, float velocity);
	virtual void handlePitchwheel (int wheelValue);
	virtual void handleAftertouch (int noteNumber, int aftertouchValue);
	virtual void handleChannelPressure (int channelPressureValue);
	virtual void handleSustainPedal (int controllerValue);
	virtual void handleSostenutoPedal (int controllerValue);
	virtual void handleSoftPedal (int controllerValue);
	virtual void handleController (int controllerNumber, int controllerValue);
	virtual void handleAllSoundOff() { }

	int lastMidiChannel { 1 };
	int lastMidiTimestamp { 0 };

	int lastPitchwheelValue { 64 };

	int lastMovedController { 0 };
	int lastControllerValue { 0 };

	bool sustainPedalDown { false };
	bool sostenutoPedalDown { false };
	bool softPedalDown { false };
};

}  // namespace lemons::midi
