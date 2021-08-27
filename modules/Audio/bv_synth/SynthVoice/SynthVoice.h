
#pragma once

#include <bv_audio_effects/bv_audio_effects.h>

namespace bav::dsp
{
template < typename SampleType >
class SynthBase;


template < typename SampleType >
class SynthVoiceBase
{
    using uint32     = juce::uint32;
    using ADSR       = juce::ADSR;
    using ADSRParams = juce::ADSR::Parameters;

    /*=================================================================================
         =================================================================================*/

public:
    SynthVoiceBase (SynthBase< SampleType >* base, double initSamplerate = 44100.0);

    virtual ~SynthVoiceBase() = default;

    void prepare (double samplerate, int blocksize);

    void release() { released(); }
    void resetRampedValues();

    void renderBlock (AudioBuffer< SampleType >& output);

    void bypassedBlock (const int numSamples);

    bool isCurrentPedalVoice() const noexcept { return isPedalPitchVoice; }
    bool isCurrentDescantVoice() const noexcept { return isDescantVoice; }

    int getCurrentMidiPan() const noexcept { return panner.getLastMidiPan(); }

    bool isKeyDown() const noexcept { return keyIsDown; }

    bool wasStartedBefore (const SynthVoiceBase& other) const noexcept { return noteOnTime < other.noteOnTime; }

    bool isPlayingButReleased() const noexcept { return playingButReleased; }

    bool isVoiceActive() const noexcept { return currentlyPlayingNote >= 0; }

    int getCurrentlyPlayingNote() const noexcept { return currentlyPlayingNote; }

    int getMidiChannel() const { return midiChannel > 0 ? midiChannel : parent->midi.getLastMidiChannel(); }

    void setTargetOutputFrequency (float newFreq);

    void setPitchGlideTime (double glideTimeSeconds);
    void togglePitchGlide (bool shouldGlide);

    /*=================================================================================
         =================================================================================*/

protected:
    friend class SynthBase< SampleType >;

    /*
            Called in the subclass to actually generate some audio at the desired frequency.
            The output buffer sent to this function will contain the number of samples desired for this frame, and your output samples should start at index 0.
        */
    virtual void renderPlease (AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate) = 0;

    virtual void prepared (double /*samplerate*/, int /*blocksize*/) { }

    virtual void released() { }

    virtual void noteCleared() { }

    virtual void bypassedBlockRecieved (float /*voicesLastOutputFreq*/, double /*currentSamplerate*/, int /*numSamples*/) { }

    /* called each time the parent recieves a new block in renderVoices(), before any calls to renderPlease() are made. This function may be called even if the voice is not currently active. */
    virtual void newBlockComing (int /*previousBlocksize*/, int /*upcomingBlocksize*/) { }

    /*=================================================================================
         =================================================================================*/

private:
    void renderInternal (int totalNumSamples);

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

    bool isVoiceOnRightNow() const;

    /*=================================================================================
         =================================================================================*/

    SynthBase< SampleType >* parent;

    ADSR adsr, quickRelease;

    bool keyIsDown {false}, playingButReleased {false}, sustainingFromSostenutoPedal {false}, isQuickFading {false};

    int   currentlyPlayingNote {-1}, currentAftertouch {0};
    float lastRecievedVelocity {0};

    uint32 noteOnTime {0};

    bool isPedalPitchVoice {false}, isDescantVoice {false}, isDoubledByAutomatedVoice {false};

    ValueSmoother< SampleType > outputFrequency;
    double                      pitchGlideTimeSecs {0.4};
    bool                        pitchGlide {false};

    FX::MonoToStereoPanner< SampleType > panner;

    FX::SmoothedGain< SampleType, 1 > midiVelocityGain, softPedalGain, playingButReleasedGain, aftertouchGain;

    AudioBuffer< SampleType > scratchBuffer, renderingBuffer, stereoBuffer;

    int midiChannel {1};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoiceBase)
};


}  // namespace bav::dsp
