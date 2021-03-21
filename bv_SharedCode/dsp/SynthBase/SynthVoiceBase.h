

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

    
    /*=================================================================================
     =================================================================================*/


//==============================================================================
//        _        _           _  _
//     __| |  ___ | |_   __ _ (_)| | ___
//    / _` | / _ \| __| / _` || || |/ __|
//   | (_| ||  __/| |_ | (_| || || |\__ \ _  _  _
//    \__,_| \___| \__| \__,_||_||_||___/(_)(_)(_)
//
//   Beyond here be implentation details.....
//
//==============================================================================


// multiplicative smoothing cannot ever actually reach 0
#define bv_MIN_SMOOTHED_GAIN 0.0000001
#define _SMOOTHING_ZERO_CHECK(inputGain) std::max(SampleType(bv_MIN_SMOOTHED_GAIN), SampleType (inputGain))
 

    /*
        Constructor requires a pointer to the parent SynthBase object that manages this voice.
    */
    template<typename SampleType>
    SynthVoiceBase<SampleType>::SynthVoiceBase<SampleType>(SynthBase<SampleType>* base, double initSamplerate = 44100.0):
            parent(base),
            keyIsDown(false), playingButReleased(false), sustainingFromSostenutoPedal(false), isQuickFading(false), noteTurnedOff(false),
            currentlyPlayingNote(-1), currentAftertouch(0), currentOutputFreq(-1.0f),  lastRecievedVelocity(0.0f), noteOnTime(0),
            isPedalPitchVoice(false), isDescantVoice(false),
            renderingBuffer(0, 0), stereoBuffer(0, 0)
    {
        adsr        .setSampleRate (initSamplerate);
        quickRelease.setSampleRate (initSamplerate);
        quickAttack .setSampleRate (initSamplerate);
        adsr        .setParameters (parent->getCurrentAdsrParams());
        quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
        quickAttack .setParameters (parent->getCurrentQuickAttackParams());
    }
    
    
    template<typename SampleType>
    virtual void SynthVoiceBase<SampleType>::prepare (const int blocksize)
    {
        renderingBuffer.setSize (1, blocksize, true, true, true);
        stereoBuffer.setSize (2, blocksize, true, true, true);
    }
    
    
    template<typename SampleType>
    virtual void SynthVoiceBase<SampleType>::renderBlock (AudioBuffer& output)
    {
        const bool voiceIsOnRightNow = isQuickFading ? quickRelease.isActive()
                                                     : ( parent->isADSRon() ? adsr.isActive() : ! noteTurnedOff );
        if (! voiceIsOnRightNow)
        {
            clearCurrentNote();
            return;
        }
        
        jassert (output.getNumSamples() > 0 && output.getNumChannels() > 0);
        
        jassert (currentOutputFreq > 0);
        jassert (parent->getSamplerate() > 0);
        jassert (renderingBuffer.getNumChannels() > 0);
        
        const int numSamples = output.getNumSamples();
        
        //  alias buffer containing exactly the # of samples we want this frame
        AudioBuffer render (renderingBuffer.getArrayOfWritePointers(), 1, 0, numSamples);
        
        //  generate some mono audio output at the frequency currentOutputFreq.....
        renderPlease (render, currentOutputFreq, parent->getSamplerate());
        
        //  smoothed gain modulations
        midiVelocityGain.applyGain       (render, numSamples);
        softPedalGain.applyGain          (render, numSamples);
        playingButReleasedGain.applyGain (render, numSamples);
        aftertouchGain.applyGain         (render, numSamples);
        
        //  ADSR
        if (parent->isADSRon())
            adsr.applyEnvelopeToBuffer (render, 0, numSamples); // midi-triggered adsr envelope
        else
            quickAttack.applyEnvelopeToBuffer (render, 0, numSamples); // to prevent pops at start of notes if adsr is off
        
        if (isQuickFading)  // quick fade out for stopNote w/ no tail off, to prevent clicks from jumping to 0
            quickRelease.applyEnvelopeToBuffer (render, 0, numSamples);
        
        if (output.getNumChannels() == 1)
            output.addFrom (0, 0, render, 0, 0, numSamples);
        else
        {
            //  write to stereoBuffer and apply panning
            stereoBuffer.copyFrom (0, 0, render, 0, 0, numSamples);
            stereoBuffer.copyFrom (1, 0, render, 0, 0, numSamples);
            outputLeftGain.applyGain  (stereoBuffer.getWritePointer(0), numSamples);
            outputRightGain.applyGain (stereoBuffer.getWritePointer(1), numSamples);
            
            //  add (!) to output
            for (int chan = 0; chan < 2; ++chan)
                output.addFrom (chan, 0, stereoBuffer, chan, 0, numSamples);
        }
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::bypassedBlock (const int numSamples)
    {
        midiVelocityGain.skip (numSamples);
        softPedalGain.skip (numSamples);
        playingButReleasedGain.skip (numSamples);
        aftertouchGain.skip (numSamples);
        outputLeftGain.skip (numSamples);
        outputRightGain.skip (numSamples);
        bypassedBlockRecieved (numSamples);
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::resetRampedValues (int blocksize)
    {
        midiVelocityGain.reset (blocksize);
        softPedalGain.reset (blocksize);
        playingButReleasedGain.reset (blocksize);
        aftertouchGain.reset (blocksize);
        outputLeftGain.reset (blocksize);
        outputRightGain.reset (blocksize);
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::clearCurrentNote()
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
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::startNote (const int midiPitch,  const float velocity,
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
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::stopNote (const float velocity, const bool allowTailOff)
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
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::updateSampleRate (const double newSamplerate)
    {
        adsr        .setSampleRate (newSamplerate);
        quickRelease.setSampleRate (newSamplerate);
        quickAttack .setSampleRate (newSamplerate);
        
        adsr        .setParameters (parent->getCurrentAdsrParams());
        quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
        quickAttack .setParameters (parent->getCurrentQuickAttackParams());
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::setKeyDown (bool isNowDown)
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
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::setPan (int newPan)
    {
        newPan = juce::jlimit(0, 127, newPan);
        
        if (panner.getLastMidiPan() == newPan)
            return;
        
        panner.setMidiPan (newPan);
        
        outputLeftGain.setTargetValue (_SMOOTHING_ZERO_CHECK (panner.getLeftGain()));
        outputRightGain.setTargetValue (_SMOOTHING_ZERO_CHECK (panner.getRightGain()));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::softPedalChanged (bool isDown)
    {
        if (isDown)
            softPedalGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getSoftPedalMultiplier()));
        else
            softPedalGain.setTargetValue (SampleType(1.0));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::aftertouchChanged (const int newAftertouchValue)
    {
        currentAftertouch = newAftertouchValue;
        
        constexpr auto inv127 = SampleType (1.0 / 127.0);
        
        if (parent->isAftertouchGainOn())
            aftertouchGain.setTargetValue (_SMOOTHING_ZERO_CHECK (newAftertouchValue * inv127));
        else
            aftertouchGain.setTargetValue (SampleType(1.0));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::setVelocityMultiplier (const float newMultiplier)
    {
        midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK(newMultiplier));
    }


#undef bv_MIN_SMOOTHED_GAIN
#undef _SMOOTHING_ZERO_CHECK

    
    ///explicit class instantiations
    template class SynthVoiceBase<float>;
    template class SynthVoiceBase<double>;
    

}  // namespace
