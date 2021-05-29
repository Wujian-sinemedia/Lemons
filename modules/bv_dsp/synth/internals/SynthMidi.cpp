

namespace bav::dsp
{
/*
 Processes all the events in the given MidiBuffer, and returns the synth's sggregate MIDI output in-place.
 */
template < typename SampleType >
void SynthBase< SampleType >::processMidi (MidiBuffer& midiMessages)
{
    aggregateMidiBuffer.clear();

    midiInputStorage.clear();
    midiInputStorage.addEvents (midiMessages, 0, midiMessages.getLastEventTime(), 0);

    std::for_each (midiMessages.findNextSamplePosition (0),
                   midiMessages.cend(),
                   [&] (const juce::MidiMessageMetadata& meta)
                   { handleMidiEvent (meta.getMessage(), meta.samplePosition); });

    midiMessages.swapWith (aggregateMidiBuffer);

    lastMidiTimeStamp = -1;
    midiInputStorage.clear();
}

/*
 Processes a single MIDI event at a time (for public use of this class).
 */
template < typename SampleType >
void SynthBase< SampleType >::processMidiEvent (const MidiMessage& m)
{
    const auto timestamp = int (m.getTimeStamp());
    midiInputStorage.addEvent (m, timestamp);
    handleMidiEvent (m, timestamp);
}


/*
 This function is the top-level function used for internally processing any given midi event (not for public use).
 */
template < typename SampleType >
void SynthBase< SampleType >::handleMidiEvent (const MidiMessage& m, const int samplePosition)
{
    lastMidiChannel   = m.getChannel();
    lastMidiTimeStamp = samplePosition - 1;  // this variable is always incremented when assigned to a new midi output event...

    if (m.isNoteOn())
    {
        lastNoteOnCounter = uint32 (lastMidiTimeStamp);
        noteOn (m.getNoteNumber(), m.getFloatVelocity(), true, lastMidiChannel);
        pitchCollectionChanged();
    }
    else if (m.isNoteOff())
    {
        noteOff (m.getNoteNumber(), m.getFloatVelocity(), true, true);
        pitchCollectionChanged();
    }
    else if (m.isAllNotesOff() || m.isAllSoundOff())
        allNotesOff (false);
    else if (m.isPitchWheel())
        handlePitchWheel (juce::jmap (m.getPitchWheelValue(), 0, 16383, 0, 127));
    else if (m.isAftertouch())
        handleAftertouch (m.getNoteNumber(), m.getAfterTouchValue());
    else if (m.isChannelPressure())
        handleChannelPressure (m.getChannelPressureValue());
    else if (m.isController())
        handleController (m.getControllerNumber(), m.getControllerValue());
}


/*
      _  _  ___ _____ ___    _____   _____ _  _ _____ ___
     | \| |/ _ \_   _| __|  | __\ \ / / __| \| |_   _/ __|
     | .` | (_) || | | _|   | _| \ V /| _|| .` | | | \__ \
     |_|\_|\___/ |_| |___|  |___| \_/ |___|_|\_| |_| |___/
 
     Functions for handling MIDI note events
 */

/*
 Any time a note-on event is requested, this function is called. This function takes care of determining which voice to use, and outputting the appropriate MIDI event(s) to the aggregateMidiBuffer.
 */
template < typename SampleType >
void SynthBase< SampleType >::noteOn (const int midiPitch, const float velocity, const bool isKeyboard, const int midiChannel)
{
    // N.B. the `isKeyboard` flag should be true if this note on event was triggered directly from the plugin's midi input; this flag should be false if this note event was automatically triggered by any internal function of Imogen (descant, pedal pitch, etc)

#if BV_USE_MTS_ESP
    if (MTS_ShouldFilterNote (mtsClient, char (midiPitch), char (midiChannel)))
    {
        pedal.setNoteToOff();
        descant.setNoteToOff();
        return;
    }
#endif

    Voice* voice = nullptr;

    if (auto* prevVoice = getVoicePlayingNote (midiPitch))
    {
        voice = prevVoice;  // retrigger the same voice with the new velocity
    }
    else
    {
        const bool isStealing =
            isKeyboard ? shouldStealNotes : false;  // never steal voices for automated note events, only for keyboard triggered events

        if (auto* newVoice = findFreeVoice (isStealing))
        {
            voice = newVoice;
        }
        else  // no voice available for this note :(
        {
            pedal.turnNoteOffIfOn();
            descant.turnNoteOffIfOn();
            return;
        }
    }

    startVoice (voice, midiPitch, velocity, isKeyboard, midiChannel);
}


/*
 Any time a note-off event is requested, this function is called. This function takes care of determining which voice to stop, and outputting the appropriate MIDI event(s) to the aggregateMidiBuffer.
 */
template < typename SampleType >
void SynthBase< SampleType >::noteOff (const int midiNoteNumber, const float velocity, const bool allowTailOff, const bool isKeyboard)
{
    // N.B. the `isKeyboard` flag should be true if this note off event was triggered directly from the plugin's midi input; this flag should be false if this note event was automatically triggered by any internal function of Imogen (descant, latch, etc)

    auto* voice = getVoicePlayingNote (midiNoteNumber);

    if (voice == nullptr)
    {
        pedal.turnNoteOffIfOn();
        descant.turnNoteOffIfOn();
        return;
    }

    if (isKeyboard)
    {
        if (latchIsOn)
        {
            voice->setKeyDown (false);
        }
        else
        {
            if (! (sustainPedalDown || voice->sustainingFromSostenutoPedal))
                stopVoice (voice, velocity, allowTailOff);
            else
                voice->setKeyDown (false);
        }

        return;
    }

    // if we get here, this is an automated note-off event...

    if (! voice->isKeyDown())  // for automated note-off events, only actually stop the voice if its keyboard key isn't currently down
    {
        stopVoice (voice, velocity, allowTailOff);
        return;
    }

    // we're processing an automated note-off event, but the voice's keyboard key is still being held
    pedal.autoNoteOffKeyboardKeyHeld (midiNoteNumber);
    descant.autoNoteOffKeyboardKeyHeld (midiNoteNumber);
}


/*
 This function actually starts a given voice. Pass a nullptr to the first argument to indicate that a note-on event was requested, but an available voice could not be found (ie, if stealing is disbled, etc)
 */
template < typename SampleType >
void SynthBase< SampleType >::startVoice (Voice* voice, const int midiPitch, const float velocity, const bool isKeyboard, const int midiChannel)
{
    if (voice == nullptr) return;

    const auto prevNote = voice->getCurrentlyPlayingNote();
    const bool wasStolen =
        voice->isVoiceActive();  // we know the voice is being "stolen" from another note if it was already on before getting this start command
    const bool sameNoteRetriggered = wasStolen && prevNote == midiPitch;

    // aftertouch value based on how much the new velocity has changed from the voice's last recieved velocity (only used if applicable)
    const auto aftertouch = juce::jlimit (0, 127, juce::roundToInt ((velocity - voice->getLastRecievedVelocity()) * 127.0f));

    //  if the same note is being retriggered, leave the timestamp the same as it was
    const auto timestamp = sameNoteRetriggered ? voice->noteOnTime : ++lastNoteOnCounter;

    if (! sameNoteRetriggered)  // only output note events if it's not the same note being retriggered
    {
        if (wasStolen)
        {
            // output a note off for the voice's previous note
            aggregateMidiBuffer.addEvent (MidiMessage::noteOff (midiChannel, prevNote, velocity), lastMidiTimeStamp);
        }

        voice->aftertouchChanged (0);

        aggregateMidiBuffer.addEvent (MidiMessage::noteOn (midiChannel, midiPitch, velocity), ++lastMidiTimeStamp);
    }
    else if (aftertouch != voice->currentAftertouch)
    {
        // same note retriggered: output aftertouch / channel pressure

        voice->aftertouchChanged (aftertouch);

        if (aftertouchGainIsOn)
            aggregateMidiBuffer.addEvent (MidiMessage::aftertouchChange (midiChannel, midiPitch, aftertouch), ++lastMidiTimeStamp);
        else
            updateChannelPressure (aftertouch);
    }
    else
    {
        voice->aftertouchChanged (0);
    }

    if (midiPitch < lowestPannedNote)  // set pan to 64 if note is below panning threshold
    {
        if (wasStolen) panner.panValTurnedOff (voice->getCurrentMidiPan());

        voice->setPan (64);
    }
    else if (! wasStolen)  // don't change pan if voice was stolen
    {
        voice->setPan (panner.getNextPanVal());
    }

    const bool isPedal   = pedal.isAutomatedPitch (midiPitch);
    const bool isDescant = descant.isAutomatedPitch (midiPitch);
    const bool keydown   = isKeyboard ? true : voice->isKeyDown();

    voice->startNote (midiPitch, velocity, timestamp, keydown, isPedal, isDescant, midiChannel);
}


/*
 This function actually stops a given voice.
 */
template < typename SampleType >
void SynthBase< SampleType >::stopVoice (Voice* voice, const float velocity, const bool allowTailOff)
{
    if (voice == nullptr) return;

    if (sustainPedalDown || voice->sustainingFromSostenutoPedal) return;

    const auto note = voice->getCurrentlyPlayingNote();

    aggregateMidiBuffer.addEvent (MidiMessage::noteOff (voice->getMidiChannel(), note, velocity), ++lastMidiTimeStamp);

    const bool isPedal   = voice->isCurrentPedalVoice();
    const bool isDescant = voice->isCurrentDescantVoice();

    if (isPedal) pedal.setNoteToOff();
    if (isDescant) descant.setNoteToOff();

    const bool shouldStopPedal   = voice->isDoubledByAutomatedVoice && ! isPedal;
    const bool shouldStopDescant = voice->isDoubledByAutomatedVoice && ! isDescant;

    voice->stopNote (velocity, allowTailOff);

    if (shouldStopPedal)
        pedal.turnNoteOffIfOn();

    if (shouldStopDescant)
        descant.turnNoteOffIfOn();
}


/*
 Turns off all currently active notes.
 */
template < typename SampleType >
void SynthBase< SampleType >::allNotesOff (const bool allowTailOff, const float velocity)
{
    for (auto* voice : voices)
        if (voice->isVoiceActive()) stopVoice (voice, velocity, allowTailOff);

    panner.reset();
}


/*
 Turns off all notes whose keyboard keys aren't being held down anymore.
 */
template < typename SampleType >
void SynthBase< SampleType >::turnOffAllKeyupNotes (const bool  allowTailOff,
                                                    const bool  includePedalPitchAndDescant,
                                                    const float velocity,
                                                    const bool  overrideSostenutoPedal)
{
    for (auto* voice : voices)
        if ((voice->isVoiceActive() && ! voice->isKeyDown())
            && (includePedalPitchAndDescant || (! (voice->isCurrentPedalVoice() || voice->isCurrentDescantVoice())))
            && (overrideSostenutoPedal || ! voice->sustainingFromSostenutoPedal))
        {
            stopVoice (voice, velocity, allowTailOff);
        }
}


/*
      ___ ___  _  _ _____ ___  ___  _    _    ___ ___    _____   _____ _  _ _____ ___
     / __/ _ \| \| |_   _| _ \/ _ \| |  | |  | __| _ \  | __\ \ / / __| \| |_   _/ __|
    | (_| (_) | .` | | | |   / (_) | |__| |__| _||   /  | _| \ V /| _|| .` | | | \__ \
     \___\___/|_|\_| |_| |_|_\\___/|____|____|___|_|_\  |___| \_/ |___|_|\_| |_| |___/
 
     Functions for handling controller and other miscellaneous MIDI events
 */

/*
 Any controller-type MIDI message is processed by this function.
 */
template < typename SampleType >
void SynthBase< SampleType >::handleController (const int controllerNumber, int controllerValue)
{
    jassert (controllerValue >= 0 && controllerValue <= 127);

    lastMovedControllerInfo.controllerNumber = controllerNumber;
    lastMovedControllerInfo.controllerValue  = controllerValue;

    switch (controllerNumber)
    {
        case 0x40 : handleSustainPedal (controllerValue); return;
        case 0x42 : handleSostenutoPedal (controllerValue); return;
        case 0x43 : handleSoftPedal (controllerValue); return;
        default : return;
    }
}


/*
 Handles pitch wheel events.
 */
template < typename SampleType >
void SynthBase< SampleType >::handlePitchWheel (int wheelValue)
{
    jassert (wheelValue >= 0 && wheelValue <= 127);

    if (lastPitchWheelValue == wheelValue) return;

    aggregateMidiBuffer.addEvent (MidiMessage::pitchWheel (lastMidiChannel, wheelValue), ++lastMidiTimeStamp);

    lastPitchWheelValue = wheelValue;
    bendTracker.newPitchbendRecieved (wheelValue);

    for (auto* voice : voices)
        if (voice->isVoiceActive()) voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
}


/*
 For an incoming aftertouch value, finds the applicable voice and relays the aftertouch control value.
 */
template < typename SampleType >
void SynthBase< SampleType >::handleAftertouch (int midiNoteNumber, int aftertouchValue)
{
    jassert (midiNoteNumber >= 0 && midiNoteNumber <= 127);
    jassert (aftertouchValue >= 0 && aftertouchValue <= 127);

    aggregateMidiBuffer.addEvent (MidiMessage::aftertouchChange (lastMidiChannel, midiNoteNumber, aftertouchValue), ++lastMidiTimeStamp);

    for (auto* voice : voices)
    {
        if (voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiNoteNumber)
        {
            voice->aftertouchChanged (aftertouchValue);
            break;
        }
    }
}


/*
 For an incoming channel pressure value, relays the control value to all voices.
 */
template < typename SampleType >
void SynthBase< SampleType >::handleChannelPressure (int channelPressureValue)
{
    jassert (channelPressureValue >= 0 && channelPressureValue <= 127);

    aggregateMidiBuffer.addEvent (MidiMessage::channelPressureChange (lastMidiChannel, channelPressureValue), ++lastMidiTimeStamp);

    for (auto* voice : voices)
        voice->aftertouchChanged (channelPressureValue);
}


/*
 This function is used to output channel pressure data. Any time a new aftertouch value is triggered internally by the synth, this function is called to test is the new incoming value is greater than the highest previously recieved aftertouch value of any active voice. If it is, the new aftertouch value is output as the synth's aggregate channel pressure value.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateChannelPressure (int newIncomingAftertouch)
{
    jassert (newIncomingAftertouch >= 0 && newIncomingAftertouch <= 127);

    int highestAftertouch = -1;

    for (auto* voice : voices)
    {
        if (! voice->isVoiceActive()) continue;

        const auto at = voice->currentAftertouch;

        if (at > highestAftertouch) highestAftertouch = at;
    }

    if (newIncomingAftertouch > highestAftertouch) handleChannelPressure (highestAftertouch);
}


/*
 Handles sustain pedal events.
 */
template < typename SampleType >
void SynthBase< SampleType >::handleSustainPedal (const int value)
{
    const bool isDown = (value >= 64);

    if (sustainPedalDown == isDown) return;

    aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (lastMidiChannel, 0x40, value), ++lastMidiTimeStamp);

    sustainPedalDown = isDown;

    if (! isDown && ! latchIsOn) turnOffAllKeyupNotes (false, false, 0.0f, false);
}


/*
 Handles sostenuto pedal events.
 */
template < typename SampleType >
void SynthBase< SampleType >::handleSostenutoPedal (const int value)
{
    const bool isDown = (value >= 64);

    if (sostenutoPedalDown == isDown) return;

    aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (lastMidiChannel, 0x42, value), ++lastMidiTimeStamp);

    sostenutoPedalDown = isDown;

    if (isDown && ! latchIsOn)
    {
        for (auto* voice : voices)
            if (voice->isVoiceActive() && ! voice->isPedalPitchVoice && ! voice->isDescantVoice) voice->sustainingFromSostenutoPedal = true;
    }
    else
    {
        turnOffAllKeyupNotes (false, false, 0.0f, true);
    }
}


/*
 Handles soft pedal events.
 */
template < typename SampleType >
void SynthBase< SampleType >::handleSoftPedal (const int value)
{
    const bool isDown = value >= 64;

    if (softPedalDown == isDown) return;

    softPedalDown = isDown;

    aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (lastMidiChannel, 0x43, value), ++lastMidiTimeStamp);

    for (auto* voice : voices)
        voice->softPedalChanged (isDown);
}


/*
      _____  _______ ___    _      __  __ ___ ___ ___    ___ ___   _ _____ _   _ ___ ___ ___
     | __\ \/ /_   _| _ \  /_\    |  \/  |_ _|   \_ _|  | __| __| /_\_   _| | | | _ \ __/ __|
     | _| >  <  | | |   / / _ \   | |\/| || || |) | |   | _|| _| / _ \| | | |_| |   / _|\__ \
     |___/_/\_\ |_| |_|_\/_/ \_\  |_|  |_|___|___/___|  |_| |___/_/ \_\_|  \___/|_|_\___|___/
 
     Automated harmony voices, chord triggering, and other miscellaneous MIDI features
 */

/*
 Use this function to toggle the synth's "midi latch" feature.
 When latch is on/true, any recieved note offs will be ignored until latch is turned off, at which point any notes whose keyboard keys aren't still being held will be turned off.
 */
template < typename SampleType >
void SynthBase< SampleType >::setMidiLatch (const bool shouldBeOn, const bool allowTailOff)
{
    if (latchIsOn == shouldBeOn) return;

    latchIsOn = shouldBeOn;

    if (shouldBeOn) return;

    turnOffAllKeyupNotes (allowTailOff, false, ! allowTailOff, false);

    pitchCollectionChanged();
}


/*
 Pass a list of desired pitches to this function to play a chord.
 This function uses turnOnList() and turnOffList() internally.
 */
template < typename SampleType >
void SynthBase< SampleType >::playChord (const juce::Array< int >& desiredPitches, const float velocity, const bool allowTailOffOfOld)
{
    if (desiredPitches.isEmpty())
    {
        allNotesOff (allowTailOffOfOld);
        return;
    }

    // create array containing current pitches

    currentNotes.clearQuick();

    reportActiveNotes (currentNotes, false, true);

    if (currentNotes.isEmpty())
    {
        turnOnList (desiredPitches, velocity, true);
    }
    else
    {
        // 1. turn off the pitches that were previously on that are not included in the list of desired pitches

        desiredNotes.clearQuick();

        for (int note : currentNotes)
            if (! desiredPitches.contains (note)) desiredNotes.add (note);

        turnOffList (desiredNotes, ! allowTailOffOfOld, allowTailOffOfOld, true);

        // 2. turn on the desired pitches that aren't already on

        desiredNotes.clearQuick();

        for (int note : desiredPitches)
            if (! currentNotes.contains (note)) desiredNotes.add (note);

        turnOnList (desiredNotes, velocity, true);
    }
}


/*
 Turns on a specified list of pitches.
 */
template < typename SampleType >
void SynthBase< SampleType >::turnOnList (const juce::Array< int >& toTurnOn, const float velocity, const bool partOfChord)
{
    if (toTurnOn.isEmpty()) return;

    for (int note : toTurnOn)
        noteOn (note, velocity, false);

    if (! partOfChord) pitchCollectionChanged();
}


/*
 Turns off a specified list of pitches.
 */
template < typename SampleType >
void SynthBase< SampleType >::turnOffList (const juce::Array< int >& toTurnOff, const float velocity, const bool allowTailOff, const bool partOfChord)
{
    if (toTurnOff.isEmpty()) return;

    for (int note : toTurnOff)
        noteOff (note, velocity, allowTailOff, false);

    if (! partOfChord) pitchCollectionChanged();
}


/*
      __  __ ___ ___  ___    __  __ ___ ___ ___    ___ _   _ _  _  ___ _____ ___ ___  _  _ ___
     |  \/  |_ _/ __|/ __|  |  \/  |_ _|   \_ _|  | __| | | | \| |/ __|_   _|_ _/ _ \| \| / __|
     | |\/| || |\__ \ (__   | |\/| || || |) | |   | _|| |_| | .` | (__  | |  | | (_) | .` \__ \
     |_|  |_|___|___/\___|  |_|  |_|___|___/___|  |_|  \___/|_|\_|\___| |_| |___\___/|_|\_|___/
 
     Miscellaneous MIDI functions
 */

/*
 Returns true if any of the synth's voices is currently active and playing the specified note.
 */
template < typename SampleType >
bool SynthBase< SampleType >::isPitchActive (const int midiPitch, const bool countRingingButReleased, const bool countKeyUpNotes) const
{
    for (auto* voice : voices)
        if ((voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch) && (countRingingButReleased || ! voice->isPlayingButReleased())
            && (countKeyUpNotes || voice->isKeyDown()))
        {
            return true;
        }

    return false;
}


/*
 Fills the passed array with the currently active pithes.
 The array will be empty if no notes are currently active.
 */
template < typename SampleType >
void SynthBase< SampleType >::reportActiveNotes (juce::Array< int >& outputArray,
                                                 const bool          includePlayingButReleased,
                                                 const bool          includeKeyUpNotes) const
{
    outputArray.clearQuick();

    for (auto* voice : voices)
        if ((voice->isVoiceActive()) && (includePlayingButReleased || ! voice->isPlayingButReleased()) && (includeKeyUpNotes || voice->isKeyDown()))
        {
            outputArray.add (voice->getCurrentlyPlayingNote());
        }

    if (! outputArray.isEmpty()) outputArray.sort();
}


/*
 This function should be called any time the aggregate collection of pitches being played by the synth changes (ie, after recieving a note event, or after triggering a chord, etc)
 */
template < typename SampleType >
void SynthBase< SampleType >::pitchCollectionChanged()
{
    pedal.apply();
    descant.apply();

    if (getNumActiveVoices() == 0) panner.reset();
}


}  // namespace bav::dsp
