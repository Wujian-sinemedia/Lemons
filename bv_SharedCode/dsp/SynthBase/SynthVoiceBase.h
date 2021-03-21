
/*
    Base class for one voice that can be used by the SynthBase to play polyphonically
*/


namespace bav::dsp
{
    
    
// multiplicative smoothing cannot ever actually reach 0
#define bv_MIN_SMOOTHED_GAIN 0.0000001
#define _SMOOTHING_ZERO_CHECK(inputGain) std::max(SampleType(bvhv_MIN_SMOOTHED_GAIN), SampleType (inputGain))
    

/// forward declaration...
template<typename SampleType>
class SynthBase;
    
    
template<typename SampleType>
class SynthVoiceBase
{
    using uint32 = juce::uint32;
    using ADSR = juce::ADSR;
    using ADSRParams = juce::ADSR::Parameters;
    
public:
    SynthVoiceBase(SynthBase* base): parent(base), keyIsDown(false), noteOnTime(0), isQuickFading(false), noteTurnedOff(false), currentAftertouch(0), sustainingFromSostenutoPedal(false), currentlyPlayingNote(-1), lastRecievedVelocity(0.0f), playingButReleased(false), currentOutputFreq(-1.0f), isPedalPitchVoice(false), isDescantVoice(false)
    {
        const double initSamplerate = 44100.0;
        
        adsr        .setSampleRate (initSamplerate);
        quickRelease.setSampleRate (initSamplerate);
        quickAttack .setSampleRate (initSamplerate);
        adsr        .setParameters (parent->getCurrentAdsrParams());
        quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
        quickAttack .setParameters (parent->getCurrentQuickAttackParams());
    }
    
    virtual ~SynthVoiceBase()
    { }
    
    bool isCurrentPedalVoice()   const noexcept { return isPedalPitchVoice; }
    bool isCurrentDescantVoice() const noexcept { return isDescantVoice; }
    
    int getCurrentMidiPan() const noexcept { return panner.getLastMidiPan(); }
    
    float getLastRecievedVelocity() const noexcept { return lastRecievedVelocity; }
    
    bool isKeyDown() const noexcept { return keyIsDown; }
    
    bool wasStartedBefore (const SynthVoiceBase& other) const noexcept { return noteOnTime < other.noteOnTime; }
    
    bool isPlayingButReleased()   const noexcept { return playingButReleased; }
    
    bool isVoiceActive() const noexcept { return (currentlyPlayingNote >= 0); }
    
    int getCurrentlyPlayingNote() const noexcept { return currentlyPlayingNote; }
    
    
    virtual void renderBlock (juce::AudioBuffer<SampleType>& output)
    { }
    
    void bypassedBlock (const int numSamples)
    {
        midiVelocityGain.skip (numSamples);
        softPedalGain.skip (numSamples);
        playingButReleasedGain.skip (numSamples);
        aftertouchGain.skip (numSamples);
        outputLeftGain.skip (numSamples);
        outputRightGain.skip (numSamples);
    }
    
    void resetRampedValues (int blocksize)
    {
        midiVelocityGain.reset (blocksize);
        softPedalGain.reset (blocksize);
        playingButReleasedGain.reset (blocksize);
        aftertouchGain.reset (blocksize);
        outputLeftGain.reset (blocksize);
        outputRightGain.reset (blocksize);
    }
    
    
protected:
    void setCurrentOutputFreq (const float newFreq) { currentOutputFreq = newFreq; }
    
    // this function resets the voice's internal state & marks it as avaiable to accept a new note
    void clearCurrentNote()
    {
        lastRecievedVelocity = 0.0f;
        currentAftertouch = 0;
        currentlyPlayingNote = -1;
        noteOnTime = 0;
        isQuickFading = false;
        noteTurnedOff = true;
        keyIsDown = false;
        playingButReleased = false;
        sustainingFromSostenutoPedal = false;
        isPedalPitchVoice = false;
        isDescantVoice = false;
        
        if (quickRelease.isActive())
            quickRelease.reset();
        
        quickRelease.noteOn();
        
        if (adsr.isActive())
            adsr.reset();
        
        if (quickAttack.isActive())
            quickAttack.reset();
        
        setPan (64);
        
        resetRampedValues (juce::roundToInt (floor (synthesisBuffer.getNumSamples() * 0.5f)));
        
        noteCleared();
    }

    
    void startNote (const int midiPitch,  const float velocity,
                    const uint32 noteOnTimestamp,
                    const bool keyboardKeyIsDown = true,
                    const bool isPedal = false, const bool isDescant = false)
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
        
        midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getWeightedVelocity (velocity)));
    }
    
    
    void stopNote (const float velocity, const bool allowTailOff)
    {
        midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getWeightedVelocity (lastRecievedVelocity - velocity)));
        
        if (allowTailOff)
        {
            adsr.noteOff();
            isQuickFading = false;
        }
        else
        {
            if (! quickRelease.isActive())
                quickRelease.noteOn();
            
            isQuickFading = true;
            
            quickRelease.noteOff();
        }
        
        noteTurnedOff = true;
        keyIsDown = false;
        playingButReleased = false;
    }
    
    
    void updateSampleRate (const double newSamplerate)
    {
        adsr        .setSampleRate (newSamplerate);
        quickRelease.setSampleRate (newSamplerate);
        quickAttack .setSampleRate (newSamplerate);
        
        adsr        .setParameters (parent->getCurrentAdsrParams());
        quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
        quickAttack .setParameters (parent->getCurrentQuickAttackParams());
    }
    
    
    SynthBase* parent;  // pointer to the SynthBase object that manages this voice. Make sure you do not delete the parent object before the child!
    
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> midiVelocityGain, softPedalGain, playingButReleasedGain, outputLeftGain, outputRightGain, aftertouchGain;
    bool isQuickFading, noteTurnedOff;
    ADSR adsr;         // the main/primary ADSR driven by MIDI input to shape the voice's amplitude envelope. May be turned off by the user.
    ADSR quickRelease; // used to quickly fade out signal when stopNote() is called with the allowTailOff argument set to false, instead of jumping signal to 0
    ADSR quickAttack;  // used for if normal ADSR user toggle is OFF, to prevent jumps/pops at starts of notes.
    
    float currentOutputFreq;
    bool playingButReleased, sustainingFromSostenutoPedal;
    
    bool keyIsDown;
    
    int currentlyPlayingNote;
    int currentAftertouch;
    
    float lastRecievedVelocity;
    
    uint32 noteOnTime;
    
    bool isPedalPitchVoice, isDescantVoice;
    
    
private:
    friend class SynthBase<SampleType>;
    
    virtual void noteCleared() { }
    
    
    void setKeyDown (bool isNowDown)
    {
        keyIsDown = isNowDown;
        
        if (isNowDown)
            playingButReleased = false;
        else
        {
            if (isPedalPitchVoice || isDescantVoice)
                playingButReleased = false;
            else if (parent->isLatched())
                playingButReleased = false;
            else
                playingButReleased = isVoiceActive();
        }
        
        if (playingButReleased)
            playingButReleasedGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getPlayingButReleasedMultiplier()));
        else
            playingButReleasedGain.setTargetValue (SampleType(1.0));
    }
    
    
    void setPan (int newPan)
    {
        newPan = juce::jlimit(0, 127, newPan);
        
        if (panner.getLastMidiPan() == newPan)
            return;
        
        panner.setMidiPan (newPan);
        
        outputLeftGain.setTargetValue (_SMOOTHING_ZERO_CHECK (panner.getLeftGain()));
        outputRightGain.setTargetValue (_SMOOTHING_ZERO_CHECK (panner.getRightGain()));
    }
    
    
    void setVelocityMultiplier (const float newMultiplier) { midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK(newMultiplier)); }
    
    
    void softPedalChanged (bool isDown)
    {
        if (isDown)
            softPedalGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getSoftPedalMultiplier()));
        else
            softPedalGain.setTargetValue (SampleType(1.0));
    }
    
    
    void aftertouchChanged (const int newAftertouchValue)
    {
        currentAftertouch = newAftertouchValue;
        
        constexpr auto inv127 = SampleType (1.0 / 127.0);
        
        if (parent->isAftertouchGainOn())
            aftertouchGain.setTargetValue (_SMOOTHING_ZERO_CHECK (newAftertouchValue * inv127));
        else
            aftertouchGain.setTargetValue (SampleType(1.0));
    }
    
    void setAdsrParameters (const ADSRParams newParams) { adsr.setParameters(newParams); }
    void setQuickReleaseParameters (const ADSRParams newParams) { quickRelease.setParameters(newParams); }
    void setQuickAttackParameters  (const ADSRParams newParams) { quickAttack.setParameters(newParams); }
    
    bav::dsp::Panner panner;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthVoiceBase)
};
    
}  // namespace


#undef bv_MIN_SMOOTHED_GAIN
#undef _SMOOTHING_ZERO_CHECK
