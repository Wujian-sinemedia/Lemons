
#pragma once

namespace bav::dsp
{
/// forward declaration...
template < typename SampleType >
class SynthBase;


/*
        SynthVoiceBase: Base class for one voice that can be used by the SynthBase to play polyphonically
    */


template < typename SampleType >
class SynthVoiceBase
{
    using uint32      = juce::uint32;
    using ADSR        = juce::ADSR;
    using ADSRParams  = juce::ADSR::Parameters;
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    /*=================================================================================
         =================================================================================*/

public:
    SynthVoiceBase (SynthBase< SampleType >* base, double initSamplerate = 44100.0);

    virtual ~SynthVoiceBase() = default;

    /*
         */

    void prepare (const int blocksize);

    // this function only redirects to the subclass's released(), if it's been overridden.
    void release() { released(); }

    virtual void renderBlock (AudioBuffer& output, const int startSample, const int numSamples);

    void bypassedBlock (const int numSamples);

    void resetRampedValues (int blocksize);

    void setCurrentOutputFreq (const float newFreq) { currentOutputFreq = newFreq; }

    bool isCurrentPedalVoice() const noexcept { return isPedalPitchVoice; }
    bool isCurrentDescantVoice() const noexcept { return isDescantVoice; }

    int getCurrentMidiPan() const noexcept { return panner.getLastMidiPan(); }

    float getLastRecievedVelocity() const noexcept { return lastRecievedVelocity; }

    bool isKeyDown() const noexcept { return keyIsDown; }

    bool wasStartedBefore (const SynthVoiceBase& other) const noexcept { return noteOnTime < other.noteOnTime; }

    bool isPlayingButReleased() const noexcept { return playingButReleased; }

    bool isVoiceActive() const noexcept { return (currentlyPlayingNote >= 0); }

    int getCurrentlyPlayingNote() const noexcept { return currentlyPlayingNote; }

    int getMidiChannel() const { return midiChannel >= 0 ? midiChannel : parent->lastMidiChannel; }

    /*=================================================================================
         =================================================================================*/

protected:
    friend class SynthBase< SampleType >;

    /*
            Called in the subclass to actually generate some audio at the desired frequency.
            The output buffer sent to this function will contain the number of samples desired for this frame, and your output samples should start at index 0.
            If you need it, the start sample from the original larger buffer sent to the parent's renderVoices() is also provided. You can probably just ignore this argument.
        */
    virtual void renderPlease (AudioBuffer& output, float desiredFrequency, double currentSamplerate, int startSampleOfOrigBuffer)
    {
        juce::ignoreUnused (output, desiredFrequency, currentSamplerate, startSampleOfOrigBuffer);
    }

    // if overridden, called in the subclass when the top-level call to prepare() is made
    virtual void prepared (int blocksize) { juce::ignoreUnused (blocksize); }

    // if overridden, called in the subclass when the top-level call to release() is made
    virtual void released() { }

    // if overridden, called in the subclass any time clearCurrentNote() is called
    virtual void noteCleared() { }

    // if overridden, called in the subclass when the top-level call to bypassedBlock() is made.
    virtual void bypassedBlockRecieved (float voicesLastOutputFreq, double currentSamplerate, int numSamples)
    {
        juce::ignoreUnused (voicesLastOutputFreq, currentSamplerate, numSamples);
    }

    /* called each time the parent recieves a new block in renderVoices(), before any calls to renderPlease() are made. This function may be called even if the voice is not currently active. */
    virtual void newBlockComing (int previousBlocksize, int upcomingBlocksize) { juce::ignoreUnused (previousBlocksize, upcomingBlocksize); }

    /*=================================================================================
         =================================================================================*/

private:
    void startNote (const int    midiPitch,
                    const float  velocity,
                    const uint32 noteOnTimestamp,
                    const bool   keyboardKeyIsDown = true,
                    const bool   isPedal           = false,
                    const bool   isDescant         = false,
                    const int    midichannel       = -1);

    void stopNote (const float velocity, const bool allowTailOff);
    
    void clearCurrentNote();

    void updateSampleRate (const double newSamplerate);

    void setKeyDown (bool isNowDown);

    void setPan (int newPan);

    void setVelocityMultiplier (const float newMultiplier);

    void softPedalChanged (bool isDown);

    void aftertouchChanged (const int newAftertouchValue);

    void setAdsrParameters (const ADSRParams newParams) { adsr.setParameters (newParams); }
    void setQuickReleaseParameters (const ADSRParams newParams) { quickRelease.setParameters (newParams); }

    // smoothed gain cannot ever reach 0
    template < typename FloatType >
    SampleType BV_FORCE_INLINE smoothingZeroCheck (FloatType gain)
    {
        return std::max (minSmoothedGain, SampleType (gain));
    }

    /*=================================================================================
         =================================================================================*/
    
    SynthBase< SampleType >* parent;
    
    ADSR adsr;  // the main/primary ADSR driven by MIDI input to shape the voice's amplitude envelope. May be turned off by the user.
    ADSR quickRelease;  // used to quickly fade out signal when stopNote() is called with the allowTailOff argument set to false, instead of jumping signal to 0
    
    bool keyIsDown, playingButReleased, sustainingFromSostenutoPedal, isQuickFading;
    
    int   currentlyPlayingNote, currentAftertouch;
    float currentOutputFreq, lastRecievedVelocity;
    
    uint32 noteOnTime;
    
    bool isPedalPitchVoice, isDescantVoice;
    
    bool isDoubledByAutomatedVoice;

    bav::dsp::FX::MonoToStereoPanner< SampleType > panner;

    // gain smoothers
    juce::SmoothedValue< SampleType, juce::ValueSmoothingTypes::Multiplicative > midiVelocityGain, softPedalGain, playingButReleasedGain,
        outputLeftGain, outputRightGain, aftertouchGain;

    AudioBuffer renderingBuffer;  // mono audio will be placed in here
    AudioBuffer stereoBuffer;     // stereo audio will be placed in here

    int midiChannel;

    static constexpr auto minSmoothedGain = SampleType (0.0000001);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoiceBase)
};


}  // namespace bav::dsp
