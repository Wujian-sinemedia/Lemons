
#pragma once

#if BV_USE_MTS_ESP

#    ifdef __clang__
#        pragma clang diagnostic push
#        pragma clang diagnostic ignored "-Weverything"
#    elif defined __GNUC__
#        pragma GCC diagnostic push
#        pragma GCC diagnostic ignored "-Weverything"
#    elif defined _MSC_VER
#        pragma warning(push, 0)
#    endif

#    include <libMTSClient.h>

#    ifdef __clang__
#        pragma clang diagnostic pop
#    elif defined __GNUC__
#        pragma GCC diagnostic pop
#    elif defined _MSC_VER
#        pragma warning(pop)
#    endif

#endif /* BV_USE_MTS_ESP */

#include "internals/PanningManager/PanningManager.h"


namespace bav::dsp
{
/*
 SynthBase: Base class for a polyphonic synthesiser-style instrument.
 This class owns, manages & manipulates a collection of SynthVoiceBase (or SynthVoiceBase-derived) objects.
 */

template < typename SampleType >
class SynthBase
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;
    using MidiBuffer  = juce::MidiBuffer;
    using MidiMessage = juce::MidiMessage;
    using uint32      = juce::uint32;
    using Voice       = SynthVoiceBase< SampleType >;
    using ADSRParams  = juce::ADSR::Parameters;


public:
    SynthBase();

    virtual ~SynthBase();

    void initialize (const int initNumVoices, const double initSamplerate, const int initBlocksize);

    void prepare (const int blocksize);

    void reset();

    void setCurrentPlaybackSampleRate (const double newRate);

    void renderVoices (juce::MidiBuffer& midiMessages, juce::AudioBuffer< SampleType >& output);

    void releaseResources();

    void resetRampedValues (int blocksize);

    void bypassedBlock (const int numSamples, MidiBuffer& midiMessages);

    void processMidiEvent (const MidiMessage& m);
    void handlePitchWheel (int wheelValue);

    void playChord (const juce::Array< int >& desiredPitches, const float velocity = 1.0f, const bool allowTailOffOfOld = false);

    void allNotesOff (const bool allowTailOff, const float velocity = 1.0f);

    void turnOffAllKeyupNotes (const bool  allowTailOff,
                               const bool  includePedalPitchAndDescant,
                               const float velocity,
                               const bool  overrideSostenutoPedal);

    bool isPitchActive (const int midiPitch, const bool countRingingButReleased = false, const bool countKeyUpNotes = false) const;
    void reportActiveNotes (juce::Array< int >& outputArray, const bool includePlayingButReleased = false, const bool includeKeyUpNotes = true) const;

    int  getNumActiveVoices() const;
    int  getNumVoices() const noexcept { return voices.size(); }
    void changeNumVoices (const int newNumVoices);

    void setNoteStealingEnabled (const bool shouldSteal) noexcept { shouldStealNotes = shouldSteal; }
    void updateMidiVelocitySensitivity (int newSensitivity);
    void updatePitchbendSettings (const int rangeUp, const int rangeDown);
    void setAftertouchGainOnOff (const bool shouldBeOn) { aftertouchGainIsOn = shouldBeOn; }

    void setPedalPitch (const bool isOn);
    void setPedalPitchUpperThresh (int newThresh);
    void setPedalPitchInterval (const int newInterval);

    void setDescant (const bool isOn);
    void setDescantLowerThresh (int newThresh);
    void setDescantInterval (const int newInterval);

    /* If you are not using MTS-ESP, you can call this method to alter the master tuning of your synth. If you are using MTS-ESP, calling this method does nothing. */
    void setConcertPitchHz (const int newConcertPitchhz);

    void updateStereoWidth (int newWidth);
    void updateLowestPannedNote (int newPitchThresh);

    void setMidiLatch (const bool shouldBeOn, const bool allowTailOff = false);
    bool isLatched() const noexcept { return latchIsOn; }

    void updateADSRsettings (const float attack, const float decay, const float sustain, const float release);
    void updateQuickReleaseMs (const int newMs);

    double getSamplerate() const noexcept { return sampleRate; }

    // returns a float velocity weighted according to the current midi velocity sensitivity settings
    float getWeightedVelocity (const float inputFloatVelocity) const;

    // returns the actual frequency in Hz a HarmonizerVoice needs to output for its latest recieved midiNote, weighted for pitchbend with the current settings & pitchwheel position, then converted to frequency with the current concert pitch settings.
    float getOutputFrequency (const int midipitch, const int midiChannel = -1) const;

    bool isSustainPedalDown() const noexcept { return midi.isSustainPedalDown(); }
    bool isSostenutoPedalDown() const noexcept { return midi.isSostenutoPedalDown(); }
    bool isSoftPedalDown() const noexcept { return midi.isSoftPedalDown(); }
    bool isAftertouchGainOn() const noexcept { return aftertouchGainIsOn; }

    float getPlayingButReleasedMultiplier() const noexcept { return playingButReleasedMultiplier; }
    void  setPlayingButReleasedMultiplier (float newGain) { playingButReleasedMultiplier = newGain; }

    float getSoftPedalMultiplier() const noexcept { return softPedalMultiplier; }
    void  setSoftPedalMultiplier (float newGain) { softPedalMultiplier = newGain; }

    ADSRParams getCurrentAdsrParams() const noexcept { return adsrParams; }
    ADSRParams getCurrentQuickReleaseParams() const noexcept { return quickReleaseParams; }

    int getLastBlocksize() const noexcept { return lastBlocksize; }


    /* MTS-ESP supports tuning tables in which certain pitches may be filtered out (note ons for these pitches are simply ignored). This function always returns false if you are not using MTS-ESP. */
    virtual bool shouldFilterNote (int midiNote, int midiChannel = -1) const;

    /* returns true if your synth is currently connected to an MTS-ESP master plugin. Returns false if your synth is not using MTS-ESP. */
    bool isConnectedToMtsEsp() const noexcept;

    /* if connected to MTS-ESP, queries the master plugin for the name of the current scale. If not using MTS-ESP or not connected to a master plugin, returns an empty String. */
    juce::String getScaleName() const;
    
    auto getLastMovedControllerInfo() const { return midi.getLastMovedCCinfo(); }

protected:
    friend class SynthVoiceBase< SampleType >;

    // if overridden, called in the subclass when the top-level call to initialize() is made.
    virtual void initialized (const double initSamplerate, const int initBlocksize) { juce::ignoreUnused (initSamplerate, initBlocksize); }

    // if overridden, called in the subclass when the top-level call to prepare() is made.
    virtual void prepared (int blocksize) { juce::ignoreUnused (blocksize); }

    // if overridden, called in the subclass when the top-level call to reset() is made.
    virtual void resetTriggered() { }

    // if overridden, called in the subclassed whenever the samplerate is changed.
    virtual void samplerateChanged (double newSamplerate) { juce::ignoreUnused (newSamplerate); }

    // if overridden, called in the subclass when the top-level call to releaseResources() is made.
    virtual void release() { }

    // this method should return an instance of your synth's voice subclass
    virtual Voice* createVoice();

private:
    void renderVoicesInternal (AudioBuffer& output, const int startSample, const int numSamples);

    void addNumVoices (const int voicesToAdd);
    void removeNumVoices (const int voicesToRemove);
    void numVoicesChanged();

    void noteOn (const int midiPitch, const float velocity, const bool isKeyboard = true, const int midiChannel = -1);
    void noteOff (const int midiNoteNumber, const float velocity, const bool allowTailOff, const bool isKeyboard = true);
    void startVoice (Voice* voice, const int midiPitch, const float velocity, const bool isKeyboard, const int midiChannel = -1);
    void stopVoice (Voice* voice, const float velocity, const bool allowTailOff);

    void handleAftertouch (int midiNoteNumber, int aftertouchValue);
    void handleChannelPressure (int channelPressureValue);
    void updateChannelPressure (int newIncomingAftertouch);
    void handleSustainPedal (const int value);
    void handleSostenutoPedal (const int value);
    void handleSoftPedal (const int value);

    void turnOnList (const juce::Array< int >& toTurnOn, const float velocity, const bool partOfChord = false);
    void turnOffList (const juce::Array< int >& toTurnOff, const float velocity, const bool allowTailOff, const bool partOfChord = false);

    // this function should be called any time the collection of pitches is changed (ie, with regular keyboard input, on each note on/off, or for chord input, once after each chord is triggered). Used for things like pedal pitch, descant, etc
    void pitchCollectionChanged();

    // voice allocation
    Voice* findFreeVoice (const bool stealIfNoneAvailable);
    Voice* findVoiceToSteal();

    Voice* getVoicePlayingNote (const int midiPitch) const;

    /*==============================================================================================================
     ===============================================================================================================*/

    juce::OwnedArray< Voice > voices;

    bool latchIsOn {false};

    juce::Array< int > currentNotes;
    juce::Array< int > desiredNotes;

    ADSRParams adsrParams;
    ADSRParams quickReleaseParams;

    double sampleRate {0.0};
    
    bool shouldStealNotes {true};

    synth_helpers::PanningManager panner;
    int                           lowestPannedNote {0};

    bav::midi::VelocityHelper  velocityConverter;
    bav::midi::PitchBendHelper bendTracker;

#if BV_USE_MTS_ESP
    MTSClient* mtsClient = nullptr;
#else
    bav::midi::PitchConverter pitchConverter;
#endif

    bool aftertouchGainIsOn {true};

    float playingButReleasedMultiplier;

    float softPedalMultiplier;  // the multiplier by which each voice's output will be multiplied when the soft pedal is down

    juce::Array< Voice* > usableVoices;  // this array is used to sort the voices when a 'steal' is requested

    int lastBlocksize;

    MidiBuffer aggregateMidiBuffer;  // this midi buffer will be used to collect the harmonizer's aggregate MIDI output
    MidiBuffer midiInputStorage;     // each block of midi that comes in is stored in here so we can refer to it later

    //--------------------------------------------------
    
    class MidiProcessor :   public midi::MidiProcessor
    {
    public:
        MidiProcessor (SynthBase& s): synth(s) { }
        
    private:
        void handleNoteOn  (int midiPitch, float velocity)
        {
            synth.noteOn (midiPitch, velocity, true, getLastMidiChannel());
        }
        
        void handleNoteOff (int midiPitch, float velocity)
        {
            synth.noteOff (midiPitch, velocity, true, getLastMidiChannel());
        }
        
        void handlePitchwheel (int wheelValue)
        {
            synth.handlePitchWheel (wheelValue);
        }
        
        void handleAftertouch (int noteNumber, int aftertouchValue)
        {
            synth.handleAftertouch (noteNumber, aftertouchValue);
        }
        
        void handleChannelPressure (int channelPressureValue)
        {
            synth.handleChannelPressure (channelPressureValue);
        }
        
        void handleSustainPedal (int controllerValue)
        {
            synth.handleSustainPedal (controllerValue);
        }
        
        void handleSostenutoPedal (int controllerValue)
        {
            synth.handleSostenutoPedal (controllerValue);
        }
        
        void handleSoftPedal (int controllerValue)
        {
            synth.handleSoftPedal (controllerValue);
        }
        
        void handleAllSoundOff()
        {
            synth.allNotesOff (false);
        }
        
        SynthBase& synth;
    };
    
    MidiProcessor midi {*this};

    //--------------------------------------------------

    class AutomatedHarmonyVoice
    {
    public:
        AutomatedHarmonyVoice (SynthBase& synthToUse, bool shiftUp);

        void apply();

        void setEnabled (bool shouldBeEnabled);
        void setThreshold (int newThresh);
        void setInterval (int newInterval);

        void turnNoteOffIfOn();
        void setNoteToOff() { lastPitch = -1; }

        bool isAutomatedPitch (int midiNote);

        // call this function when processing an automated note-off and the voice's keyboard key is still being held
        void autoNoteOffKeyboardKeyHeld (int midiNote);

        Voice* getVoice();

    private:
        const bool shiftingUp;

        bool isOn {false};
        int  lastPitch {-1};
        int  thresh {0};
        int  interval {12};

    private:
        SynthBase& synth;
    };

    AutomatedHarmonyVoice pedal {*this, false};
    AutomatedHarmonyVoice descant {*this, true};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthBase)
};


}  // namespace bav::dsp
