
namespace bav::dsp
{
/*
 Sets the frequency in Hz corresponding to midi note 69 (A4). This will usually be 440.
 Setting this to values higher or lower than 440 will effective detune the entire synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::setConcertPitchHz (const int newConcertPitchhz)
{
#if BV_USE_MTS_ESP
    juce::ignoreUnused (newConcertPitchhz);
#else
    jassert (newConcertPitchhz > 0);

    if (pitchConverter.getCurrentConcertPitchHz() == newConcertPitchhz) return;

    pitchConverter.setConcertPitchHz (newConcertPitchhz);

    for (auto* voice : voices)
        if (voice->isVoiceActive()) voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
#endif
}


/*
 Sets the synth's sensitivity to midi velocity.
 At 100, each voice's volume is entirely dependant on the velocity of its note-on message and subsequent aftertouch messages.
 At 0, velocity and aftertouch have no effect on the voices' volume.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateMidiVelocitySensitivity (int newSensitivity)
{
    jassert (newSensitivity >= 0 && newSensitivity <= 100);

    const auto newSens = newSensitivity / 100.0f;

    if (velocityConverter.getSensitivity() == newSens) return;

    velocityConverter.setSensitivity (newSens);

    for (auto* voice : voices)
        if (voice->isVoiceActive())
            voice->setVelocityMultiplier (velocityConverter.getGainForVelocity (voice->getLastRecievedVelocity()));
}


/*
 Sets the range of the pitch wheel up and down, in semitones.
 */
template < typename SampleType >
void SynthBase< SampleType >::updatePitchbendSettings (const int rangeUp, const int rangeDown)
{
    if ((bendTracker.getCurrentRangeUp() == rangeUp) && (bendTracker.getCurrentRangeDown() == rangeDown)) return;

    bendTracker.setRange (rangeUp, rangeDown);

    if (midi.getLastPitchwheelValue() == 64) return;

    for (auto* voice : voices)
        if (voice->isVoiceActive()) voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
}


/*
  ___ _____ ___ ___ ___ ___    __      _____ ___ _____ _  _
 / __|_   _| __| _ \ __/ _ \   \ \    / /_ _|   \_   _| || |
 \__ \ | | | _||   / _| (_) |   \ \/\/ / | || |) || | | __ |
 |___/ |_| |___|_|_\___\___/     \_/\_/ |___|___/ |_| |_||_|
 
 Parameters to control the stereo image of the synth's output
 */

/*
 Updates the stereo width of the synthesizer as a whole.
 At 100, the active voices will be evenly spread across the entire stereo field. At 0, the synth will produce mono output.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateStereoWidth (int newWidth)
{
    jassert (newWidth >= 0 && newWidth <= 100);

    if (panner.getCurrentStereoWidth() == newWidth) return;

    panner.updateStereoWidth (newWidth);

    for (auto* voice : voices)
    {
        if (! voice->isVoiceActive()) continue;

        if (voice->getCurrentlyPlayingNote() < lowestPannedNote)
        {
            const int currentPan = voice->getCurrentMidiPan();

            if (currentPan != 64)
            {
                panner.panValTurnedOff (currentPan);
                voice->setPan (64);
            }
        }
        else
        {
            voice->setPan (panner.getClosestNewPanValFromOld (voice->getCurrentMidiPan()));
        }
    }
}


/*
 Sets the lowest midi pitch that will be panned. Pitches below this threshold will always be placed dead center.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateLowestPannedNote (int newPitchThresh)
{
    jassert (newPitchThresh >= 0 && newPitchThresh <= 127);

    const auto prevLowestnote = lowestPannedNote;

    if (prevLowestnote == newPitchThresh) return;

    lowestPannedNote = newPitchThresh;

    for (auto* voice : voices)
    {
        if (! voice->isVoiceActive()) continue;

        const auto note       = voice->getCurrentlyPlayingNote();
        const auto currentPan = voice->getCurrentMidiPan();

        if (note < newPitchThresh)
        {
            if (currentPan != 64)
            {
                panner.panValTurnedOff (currentPan);
                voice->setPan (64);
            }
        }
        else if (
            note
            < prevLowestnote)  // because we haven't updated the lowestPannedNote member variable yet, voices with pitches higher than newPitchThresh but lower than lowestPannedNote are the voices that now qualify for panning
        {
            if (currentPan == 64) voice->setPan (panner.getNextPanVal());
        }
    }
}


/*
    _   ___  ___ ___ _ ___
   /_\ |   \/ __| _ ( ) __|
  / _ \| |) \__ \   //\__ \
 /_/ \_\___/|___/_|_\ |___/
 
 Parameters to control the synth's various ADSRs
 */

/*
 Updates the settings for the main midi-triggered ADSR.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateADSRsettings (const float attack, const float decay, const float sustain, const float release)
{
    // attack/decay/release time in SECONDS; sustain ratio 0.0 - 1.0

    adsrParams.attack  = attack;
    adsrParams.decay   = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    for (auto* voice : voices)
        voice->setAdsrParameters (adsrParams);
}


/*
 Updates the synth's quick release time, in milliseconds.
 This is used to prevent clicks when stopping notes with no tail off, etc.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateQuickReleaseMs (const int newMs)
{
    const auto desiredR = newMs * 0.001f;

    quickReleaseParams.release = desiredR;

    for (auto* voice : voices)
        voice->setQuickReleaseParameters (quickReleaseParams);
}


/*
  ___ ___ ___   _   _       ___ ___ _____ ___ _  _
 | _ \ __|   \ /_\ | |     | _ \_ _|_   _/ __| || |
 |  _/ _|| |) / _ \| |__   |  _/| |  | || (__| __ |
 |_| |___|___/_/ \_\____|  |_| |___| |_| \___|_||_|
 
 Parameters to control the automated pedal pitch feature
 */

/*
 Toggles pedal pitch on or off.
 */
template < typename SampleType >
void SynthBase< SampleType >::setPedalPitch (const bool isOn)
{
    pedal.setEnabled (isOn);
}


/*
 Sets the interval, in semitones, at which the pedal pitch voice will double the lowest played keyboard note.
 EG. 12 will make the pedal pitch feature double the lowest played note an octave lower, etc.
 */
template < typename SampleType >
void SynthBase< SampleType >::setPedalPitchInterval (const int newInterval)
{
    pedal.setInterval (newInterval);
}


/*
 Sets the pitch threshold for the pedal pitch feature.
 The pedal pitch voice will only be created if the lowest played keyboard key is below the pitch set here.
 */
template < typename SampleType >
void SynthBase< SampleType >::setPedalPitchUpperThresh (int newThresh)
{
    pedal.setThreshold (newThresh);
}


/*
  ___  ___ ___  ___   _   _  _ _____
 |   \| __/ __|/ __| /_\ | \| |_   _|
 | |) | _|\__ \ (__ / _ \| .` | | |
 |___/|___|___/\___/_/ \_\_|\_| |_|
 
 Parameters to control the automated descant feature
 */

/*
 Toggles descant on or off.
 */
template < typename SampleType >
void SynthBase< SampleType >::setDescant (const bool isOn)
{
    descant.setEnabled (isOn);
}


/*
 Sets the interval, in semitones, at which the descant voice will double the highest played keyboard note.
 EG. 12 will make the descant feature double the highest played note an octave higher, etc.
 */
template < typename SampleType >
void SynthBase< SampleType >::setDescantInterval (const int newInterval)
{
    descant.setInterval (newInterval);
}


/*
 Sets the pitch threshold for the descant feature.
 The descant voice will only be created if the highest played keyboard key is above the pitch set here.
 */
template < typename SampleType >
void SynthBase< SampleType >::setDescantLowerThresh (int newThresh)
{
    descant.setThreshold (newThresh);
}

}  // namespace bav::dsp
