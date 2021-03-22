
namespace bav::dsp
{
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::startNote (const int midiPitch,  const float velocity,
                                                const uint32 noteOnTimestamp,
                                                const bool keyboardKeyIsDown = true,
                                                const bool isPedal, const bool isDescant)
    {
        noteOnTime = noteOnTimestamp;
        currentlyPlayingNote = midiPitch;
        lastRecievedVelocity = velocity;
        currentOutputFreq = parent->getOutputFrequency (midiPitch);
        isQuickFading = false;
        noteTurnedOff = false;
        isPedalPitchVoice = isPedal;
        isDescantVoice = isDescant;
        
        adsr.noteOn();
        quickAttack.noteOn();
        
        if (! quickRelease.isActive())
            quickRelease.noteOn();
        
        setKeyDown (keyboardKeyIsDown);
        
        //midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getWeightedVelocity (velocity)));
    }
    
}  // namespace
