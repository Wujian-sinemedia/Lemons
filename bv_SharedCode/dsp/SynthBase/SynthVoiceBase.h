

namespace bav::dsp
{


/// forward declaration...
template<typename SampleType>
class SynthBase;
    
    
/*
    Base class for one voice that can be used by the SynthBase to play polyphonically
*/


template<typename SampleType>
class SynthVoiceBase
{
    using uint32 = juce::uint32;
    using ADSR = juce::ADSR;
    using ADSRParams = juce::ADSR::Parameters;
    using AudioBuffer = juce::AudioBuffer<SampleType>;
    
    /*=================================================================================
     =================================================================================*/
    
public:
    SynthVoiceBase(SynthBase<SampleType>* base, double initSamplerate = 44100.0);
    
    virtual ~SynthVoiceBase() = default;
    
    /*
     */
    
    // prepare is virtual, because if you don't use the default renderBlock, you may not need to initialize the renderingBuffer.
    virtual void prepare (const int blocksize);
    
    // this function only redirects to the subclass's released(), if it's been overridden.
    void release() { released(); }
    
    virtual void renderBlock (AudioBuffer& output);
    
    void bypassedBlock (const int numSamples);
    
    void resetRampedValues (int blocksize);
    
    void setCurrentOutputFreq (const float newFreq) { currentOutputFreq = newFreq; }
    
    bool isCurrentPedalVoice()   const noexcept { return isPedalPitchVoice; }
    bool isCurrentDescantVoice() const noexcept { return isDescantVoice; }
    
    int getCurrentMidiPan() const noexcept { return panner.getLastMidiPan(); }
    
    float getLastRecievedVelocity() const noexcept { return lastRecievedVelocity; }
    
    bool isKeyDown() const noexcept { return keyIsDown; }
    
    bool wasStartedBefore (const SynthVoiceBase& other) const noexcept { return noteOnTime < other.noteOnTime; }
    
    bool isPlayingButReleased()   const noexcept { return playingButReleased; }
    
    bool isVoiceActive() const noexcept { return (currentlyPlayingNote >= 0); }
    
    int getCurrentlyPlayingNote() const noexcept { return currentlyPlayingNote; }
    
    
    /*=================================================================================
     =================================================================================*/
    
protected:
    friend class SynthBase<SampleType>;
    
    // this function resets the voice's internal state & marks it as avaiable to accept a new note
    void clearCurrentNote();
    
    /*
     */
    
    SynthBase<SampleType>* parent;  // pointer to the SynthBase object that manages this voice. Make sure you do not delete the parent object before the child!
    
    ADSR adsr;         // the main/primary ADSR driven by MIDI input to shape the voice's amplitude envelope. May be turned off by the user.
    ADSR quickRelease; // used to quickly fade out signal when stopNote() is called with the allowTailOff argument set to false, instead of jumping signal to 0
    ADSR quickAttack;  // used for if normal ADSR user toggle is OFF, to prevent jumps/pops at starts of notes.
    
    bool keyIsDown, playingButReleased, sustainingFromSostenutoPedal, isQuickFading, noteTurnedOff;
    
    int currentlyPlayingNote, currentAftertouch;
    float currentOutputFreq, lastRecievedVelocity;
    
    uint32 noteOnTime;
    
    bool isPedalPitchVoice, isDescantVoice;
    
    
    /*=================================================================================
     =================================================================================*/
    
private:
    virtual void renderPlease (AudioBuffer& output, float desiredFrequency, double currentSamplerate)
    {
        juce::ignoreUnused (output, desiredFrequency, currentSamplerate);
    }
    
    // if overridden, called in the subclass when the top-level call to release() is made
    virtual void released() { }
    
    // if overridden, called in the subclass any time clearCurrentNote() is called
    virtual void noteCleared() { }
    
    // if overridden, called in the subclass when the top-level call to bypassedBlock() is made.
    virtual void bypassedBlockRecieved (int numSamples) { juce::ignoreUnused (numSamples); }
    
    void startNote (const int midiPitch,  const float velocity,
                    const uint32 noteOnTimestamp,
                    const bool keyboardKeyIsDown = true,
                    const bool isPedal = false, const bool isDescant = false);
    
    void stopNote (const float velocity, const bool allowTailOff);
    
    void updateSampleRate (const double newSamplerate);
    
    void setKeyDown (bool isNowDown);
    
    void setPan (int newPan);
    
    void setVelocityMultiplier (const float newMultiplier);
    
    void softPedalChanged (bool isDown);
    
    void aftertouchChanged (const int newAftertouchValue);
    
    void setAdsrParameters (const ADSRParams newParams) { adsr.setParameters(newParams); }
    void setQuickReleaseParameters (const ADSRParams newParams) { quickRelease.setParameters(newParams); }
    void setQuickAttackParameters  (const ADSRParams newParams) { quickAttack.setParameters(newParams); }
    
    /*=================================================================================
     =================================================================================*/
    
    bav::dsp::Panner panner;
    
    // gain smoothers
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> midiVelocityGain, softPedalGain, playingButReleasedGain, outputLeftGain, outputRightGain, aftertouchGain;
    
    AudioBuffer renderingBuffer;  // mono audio will be placed in here
    AudioBuffer stereoBuffer;     // stereo audio will be placed in here
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthVoiceBase)
};

    
}  // namespace
