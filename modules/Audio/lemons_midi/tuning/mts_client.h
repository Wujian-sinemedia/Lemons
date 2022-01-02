#pragma once

#include <libMTSClient.h>


namespace lemons::midi
{

/** @defgroup lemons_midi_tuning Tuning
    @ingroup lemons_midi
    Utilities for working with microtonalism in MIDI.
 */

/** @ingroup lemons_midi_tuning
    This class represents an MTS-ESP session client.
    You can use this class to get tuning information from a session master.
 */
class MtsClient
{
public:

	/** Converts a given MIDI note to a frequency in Hz, using the master tuning table.
	    @param midiNote The MIDI pitch to convert.
	    @param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
	 */
	template <typename T>
	float midiToFrequency (T midiNote, int midiChannel = -1) const
	{
		return static_cast<float> (MTS_NoteToFrequency (client.get(),
		                                                char (midiNote),
		                                                char (midiChannel)));
	}

	/** Converts a given frequency in Hz to a MIDI note, using the master tuning table.
	    @param frequency The frequency in Hz to convert.
	    @param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
	 */
	template <typename T>
	float frequencyToMidi (T frequency, int midiChannel = -1) const
	{
		return static_cast<float> (MTS_FrequencyToNote (client.get(),
		                                                static_cast<double> (frequency),
		                                                char (midiChannel)));
	}

	/** Returns the retune amount in semitones for a given MIDI pitch.
	    @param midiNote The MIDI pitch to get the retuning amount for.
	    @param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
	 */
	template <typename T>
	float retuneAmountInSemitones (T midiNote, int midiChannel = -1) const
	{
		return static_cast<float> (MTS_RetuningInSemitones (client.get(),
		                                                    char (midiNote),
		                                                    char (midiChannel)));
	}

	/** MTS-ESP supports "filtering" some notes out of tuning tables.
	    Call this function to detect if a certain pitch should be filtered out or ignored.
	    @param midiPitch The MIDI note to check.
	    @param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
	    @return True if the passed pitch should be ignored/filtered; false otherwise.
	 */
	bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;

	/** Returns true if this client is currently connected to an MTS-ESP session master. */
	bool isConnected() const;

	/** Returns the scale name specified by the MTS-ESP session master. This may return an empty string. */
	juce::String getScaleName() const;

	/** MTS-ESP clients can parse MIDI SysEx data to update their tuning tables. Call this function with a midi message, and if it's a SysEx, it will be parsed by the MTS client. */
	void parseMidiMessageForSysEx (const juce::MidiMessage& m) const;

private:
	struct Deleter
	{
		void operator() (MTSClient* c);
	};

	std::unique_ptr<MTSClient, Deleter> client { MTS_RegisterClient() };
};

}  // namespace lemons::midi
