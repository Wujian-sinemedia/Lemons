/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons::midi
{

struct Processor
{
	virtual ~Processor() = default;

	virtual void process (MidiBuffer& block) = 0;

	virtual void prepare (int /*maxBlocksize*/) { }

	virtual void release() { }

	virtual void reset() { }
};

/*---------------------------------------------------------------------------------------------------------------------------------------*/

class RoutingProcessor : public Processor
{
public:

	void process (MidiBuffer& block) final;

	void processMessage (const MidiMessage& message);

private:

	virtual void handleNoteOn (int midiPitch, float velocity);
	virtual void handleNoteOff (int midiPitch, float velocity);
	virtual void handlePitchwheel (int wheelValue);
	virtual void handleAftertouch (int noteNumber, int aftertouchValue);
	virtual void handleChannelPressure (int channelPressureValue);
	virtual void handleSustainPedal (int controllerValue);
	virtual void handleSostenutoPedal (int controllerValue);
	virtual void handleSoftPedal (int controllerValue);
	virtual void handleController (int controllerNumber, int controllerValue);
	virtual void handleResetAllControllers() { }
	virtual void handleAllSoundOff() { }
	virtual void handleProgramChange (int programNumber);
	virtual void handleSysEx (const juce::uint8* data, int dataSize);
	virtual void handleMetaEvent (int type, const juce::uint8* data, int dataSize);
	virtual void handleTextMetaEvent (String text);
	virtual void handleTempoMetaEvent (double ppq);
	virtual void handleTimeSignatureMetaEvent (int numerator, int denominator);
	virtual void handleKeySignatureMetaEvent (int numSharpsOrFlats, bool isSharps, bool isMajor);
	virtual void handleSongPositionEvent (int midiBeat);
	virtual void handleMachineControl (MidiMessage::MidiMachineControlCommand) { }
	virtual void handleOtherMessage (const MidiMessage&) { }
};

/*---------------------------------------------------------------------------------------------------------------------------------------*/

struct StatefulRoutingProcessor : public RoutingProcessor
{
	[[nodiscard]] int getLastPitchwheelValue() const noexcept;

	[[nodiscard]] bool isSustainPedalDown() const noexcept;
	[[nodiscard]] bool isSostenutoPedalDown() const noexcept;
	[[nodiscard]] bool isSoftPedalDown() const noexcept;

	void reset() final;

private:

	void handlePitchwheel (int wheelValue) final;
	void handleSustainPedal (int controllerValue) final;
	void handleSostenutoPedal (int controllerValue) final;
	void handleSoftPedal (int controllerValue) final;

	virtual void processPitchWheel (int) { }
	virtual void processSustainPedal (int) { }
	virtual void processSostenutoPedal (int) { }
	virtual void processSoftPedal (int) { }

	int pitchwheel { 64 };

	bool sustainPedalDown { false }, sostenutoPedalDown { false }, softPedalDown { false };
};

}  // namespace lemons::midi
