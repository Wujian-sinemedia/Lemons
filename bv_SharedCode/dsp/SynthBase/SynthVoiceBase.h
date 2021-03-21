
/*
Base class for one voice that can be used by the SynthBase to play polyphonically
*/


namespace bav::dsp
{


// multiplicative smoothing cannot ever actually reach 0
#define bv_MIN_SMOOTHED_GAIN 0.0000001
#define _SMOOTHING_ZERO_CHECK(inputGain) std::max(SampleType(bv_MIN_SMOOTHED_GAIN), SampleType (inputGain))


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
    SynthVoiceBase(SynthBase<SampleType>* base, double initSamplerate = 44100.0):
        parent(base),
        keyIsDown(false), playingButReleased(false), sustainingFromSostenutoPedal(false), isQuickFading(false), noteTurnedOff(false),
        currentlyPlayingNote(-1), currentAftertouch(0), currentOutputFreq(-1.0f),  lastRecievedVelocity(0.0f), noteOnTime(0),
        isPedalPitchVoice(false), isDescantVoice(false)
    {
        adsr        .setSampleRate (initSamplerate);
        quickRelease.setSampleRate (initSamplerate);
        quickAttack .setSampleRate (initSamplerate);
        adsr        .setParameters (parent->getCurrentAdsrParams());
        quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
        quickAttack .setParameters (parent->getCurrentQuickAttackParams());
    }
    
    virtual ~SynthVoiceBase()
    { }
    
    /*=================================================================================
    =================================================================================*/
    
    //  if you're looking for a prepare() function, it's a virtual void down in the priavate section!
    
    void release() { released(); }
    
    virtual void renderBlock (juce::AudioBuffer<SampleType>& output)
    {
        const bool voiceIsOnRightNow = isQuickFading ? quickRelease.isActive()
                                                     : ( parent->isADSRon() ? adsr.isActive() : ! noteTurnedOff );
        if (! voiceIsOnRightNow)
        {
            clearCurrentNote();
            return;
        }
        
        jassert (currentOutputFreq > 0);
        jassert (parent->getSamplerate() > 0);
        
        // generate some mono audio output at the frequency currentOutputFreq.....
        
        // an example of how the provided gain smoothers & ADSRs should be used:
        
        //  smoothed gain modulations
//        midiVelocityGain.applyGain (synthesisBuffer, numSamples);
//        softPedalGain.applyGain (synthesisBuffer, numSamples);
//        playingButReleasedGain.applyGain (synthesisBuffer, numSamples);
//        aftertouchGain.applyGain (synthesisBuffer, numSamples);
        
        //  ADSR
//        if (parent->isADSRon())
//            adsr.applyEnvelopeToBuffer (synthesisBuffer, 0, numSamples); // midi-triggered adsr envelope
//        else
//            quickAttack.applyEnvelopeToBuffer (synthesisBuffer, 0, numSamples); // to prevent pops at start of notes if adsr is off
//
//        if (isQuickFading)  // quick fade out for stopNote w/ no tail off, to prevent clicks from jumping to 0
//            quickRelease.applyEnvelopeToBuffer (synthesisBuffer, 0, numSamples);
        
        //  write to output and apply panning
//        outputBuffer.copyFrom (0, 0, synthesisBuffer, 0, 0, numSamples);
//        outputBuffer.copyFrom (1, 0, synthesisBuffer, 0, 0, numSamples);
//        outputLeftGain.applyGain (outputBuffer.getWritePointer(0), numSamples);
//        outputRightGain.applyGain (outputBuffer.getWritePointer(1), numSamples);
    }
    
    void bypassedBlock (const int numSamples)
    {
        midiVelocityGain.skip (numSamples);
        softPedalGain.skip (numSamples);
        playingButReleasedGain.skip (numSamples);
        aftertouchGain.skip (numSamples);
        outputLeftGain.skip (numSamples);
        outputRightGain.skip (numSamples);
        bypassedBlockRecieved (numSamples);
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
    
    bool isCurrentPedalVoice()   const noexcept { return isPedalPitchVoice; }
    bool isCurrentDescantVoice() const noexcept { return isDescantVoice; }
    
    int getCurrentMidiPan() const noexcept { return panner.getLastMidiPan(); }
    
    float getLastRecievedVelocity() const noexcept { return lastRecievedVelocity; }
    
    bool isKeyDown() const noexcept { return keyIsDown; }
    
    bool wasStartedBefore (const SynthVoiceBase& other) const noexcept { return noteOnTime < other.noteOnTime; }
    
    bool isPlayingButReleased()   const noexcept { return playingButReleased; }
    
    bool isVoiceActive() const noexcept { return (currentlyPlayingNote >= 0); }
    
    int getCurrentlyPlayingNote() const noexcept { return currentlyPlayingNote; }
    
    void setCurrentOutputFreq (const float newFreq) { currentOutputFreq = newFreq; }
    
    
protected:
    friend class SynthBase<SampleType>;
    
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
        
        resetRampedValues (parent->getLastBlocksize());
        
        noteCleared();
    }
    
    
    SynthBase<SampleType>* parent;  // pointer to the SynthBase object that manages this voice. Make sure you do not delete the parent object before the child!
    
    
    ADSR adsr;         // the main/primary ADSR driven by MIDI input to shape the voice's amplitude envelope. May be turned off by the user.
    ADSR quickRelease; // used to quickly fade out signal when stopNote() is called with the allowTailOff argument set to false, instead of jumping signal to 0
    ADSR quickAttack;  // used for if normal ADSR user toggle is OFF, to prevent jumps/pops at starts of notes.
    
    bool keyIsDown, playingButReleased, sustainingFromSostenutoPedal, isQuickFading, noteTurnedOff;
    
    int currentlyPlayingNote, currentAftertouch;
    float currentOutputFreq, lastRecievedVelocity;
    
    uint32 noteOnTime;
    
    bool isPedalPitchVoice, isDescantVoice;
    
    
private:
    
    // if overridden, called in the subclass when the top-level call to the SynthBase's prepare() function is called
    virtual void prepare (const int blocksize) { juce::ignoreUnused (blocksize); }
    
    // if overridden, called in the subclass when the top-level call to release() is made
    virtual void released() { }
    
    // if overridden, called in the subclass any time clearCurrentNote() is called
    virtual void noteCleared() { }
    
    // if overridden, called in the subclass when the top-level call to bypassedBlock() is made.
    virtual void bypassedBlockRecieved (int numSamples) { juce::ignoreUnused (numSamples); }
    
    
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
    
    // gain smoothers
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> midiVelocityGain, softPedalGain, playingButReleasedGain, outputLeftGain, outputRightGain, aftertouchGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthVoiceBase)
};

}  // namespace


#undef bv_MIN_SMOOTHED_GAIN
#undef _SMOOTHING_ZERO_CHECK


