
namespace bav::dsp
{
template < typename SampleType >
void SynthBase< SampleType >::setPitchGlideTime (double glideTimeSeconds)
{
    for (auto* voice : voices)
        voice->setPitchGlideTime (glideTimeSeconds);
}

template < typename SampleType >
void SynthBase< SampleType >::togglePitchGlide (bool shouldGlide)
{
    for (auto* voice : voices)
        voice->togglePitchGlide (shouldGlide);
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
            voice->setVelocityMultiplier (velocityConverter.getGainForVelocity (voice->lastRecievedVelocity));
}

/*
 Sets the range of the pitch wheel up and down, in semitones.
 */
template < typename SampleType >
void SynthBase< SampleType >::updatePitchbendRange (int rangeUp, int rangeDown)
{
    if ((pitch.bend.getRangeUp() == rangeUp) && (pitch.bend.getRangeDown() == rangeDown)) return;

    pitch.bend.setRange (rangeUp, rangeDown);

    if (midi.router.getLastPitchwheelValue() == 64) return;

    for (auto* voice : voices)
        if (voice->isVoiceActive())
            voice->setTargetOutputFrequency (pitch.getFrequencyForMidi (voice->getCurrentlyPlayingNote()));
}

template < typename SampleType >
void SynthBase< SampleType >::updatePitchbendRange (int rangeSemitones)
{
    updatePitchbendRange (rangeSemitones, rangeSemitones);
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
void SynthBase< SampleType >::updateADSRsettings (float attack, float decay, float sustain, float release)
{
    // attack/decay/release time in SECONDS; sustain ratio 0.0 - 1.0

    adsrParams.attack  = attack;
    adsrParams.decay   = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    for (auto* voice : voices)
        voice->setAdsrParameters (adsrParams);

    if (quickReleaseParams.release > adsrParams.release)
        updateQuickReleaseMs (juce::roundToInt (adsrParams.release * 1000.f));
}


/*
 Updates the synth's quick release time, in milliseconds.
 This is used to prevent clicks when stopping notes with no tail off, etc.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateQuickReleaseMs (int newMs)
{
    quickReleaseParams         = adsrParams;
    quickReleaseParams.release = newMs * 0.001f;

    for (auto* voice : voices)
        voice->setQuickReleaseParameters (quickReleaseParams);
}


}  // namespace bav::dsp
