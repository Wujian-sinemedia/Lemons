
#pragma once

#include "PanningManager/PanningManager.h"


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

    virtual void renderVoices (juce::MidiBuffer& midiMessages, juce::AudioBuffer< SampleType >& output);

    void releaseResources();

    void resetRampedValues (int blocksize);

    void bypassedBlock (const int numSamples, MidiBuffer& midiMessages);

    void processMidiEvent (const MidiMessage& m);

    void playChord (const juce::Array< int >& desiredPitches, const float velocity = 1.0f, const bool allowTailOffOfOld = false);

    void allNotesOff (const bool allowTailOff, const float velocity = 1.0f);

    void turnOffAllKeyupNotes (const bool  allowTailOff,
                               const bool  includePedalPitchAndDescant,
                               const float velocity,
                               const bool  overrideSostenutoPedal);

    bool isPitchActive (const int midiPitch, const bool countRingingButReleased = false, const bool countKeyUpNotes = false) const;

    void reportActiveNotes (juce::Array< int >& outputArray, const bool includePlayingButReleased = false, const bool includeKeyUpNotes = true) const;

    int getNumActiveVoices() const;

    int getNumVoices() const noexcept { return voices.size(); }

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

    double getSamplerate() const noexcept { return sampleRate; }

    void handlePitchWheel (int wheelValue);

    void updateQuickReleaseMs (const int newMs);

    // returns a float velocity weighted according to the current midi velocity sensitivity settings
    float getWeightedVelocity (const float inputFloatVelocity) const;

    // returns the actual frequency in Hz a HarmonizerVoice needs to output for its latest recieved midiNote, weighted for pitchbend with the current settings & pitchwheel position, then converted to frequency with the current concert pitch settings.
    float getOutputFrequency (const int midipitch, const int midiChannel = -1) const;

    bool  isSustainPedalDown() const noexcept { return sustainPedalDown; }
    bool  isSostenutoPedalDown() const noexcept { return sostenutoPedalDown; }
    bool  isSoftPedalDown() const noexcept { return softPedalDown; }
    float getSoftPedalMultiplier() const noexcept { return softPedalMultiplier; }
    float getPlayingButReleasedMultiplier() const noexcept { return playingButReleasedMultiplier; }
    bool  isAftertouchGainOn() const noexcept { return aftertouchGainIsOn; }

    ADSRParams getCurrentAdsrParams() const noexcept { return adsrParams; }
    ADSRParams getCurrentQuickReleaseParams() const noexcept { return quickReleaseParams; }

    int getLastBlocksize() const noexcept { return lastBlocksize; }


    /* MTS-ESP supports tuning tables in which certain pitches may be filtered out (note ons for these pitches are simply ignored). This function always returns false if you are not using MTS-ESP. */
    virtual bool shouldFilterNote (int midiNote, int midiChannel = -1) const;

    /* returns true if your synth is currently connected to an MTS-ESP master plugin. Returns false if your synth is not using MTS-ESP. */
    bool isConnectedToMtsEsp() const noexcept;

    /* if connected to MTS-ESP, queries the master plugin for the name of the current scale. If not using MTS-ESP or not connected to a master plugin, returns an empty String. */
    juce::String getScaleName() const;
    
    
    struct LastMovedControllerInfo
    {
        int controllerNumber = 0;
        int controllerValue  = 0;
    };

    LastMovedControllerInfo getLastMovedControllerInfo() const noexcept { return lastMovedControllerInfo; }

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

    // removes a specified # of voices, attempting to remove inactive voices first, and only removes active voices if necessary
    void removeNumVoices (const int voicesToRemove);

    /*
     Adds a specified # of voices to the synth. This is virtual because you can override this in your subclass to add instances of your subclass of SynthVoiceBase, assuming you're working with one.
     Your overridden function should call numVoicesChanged() at the end of the function.
     */
    virtual void addNumVoices (const int voicesToAdd);

    // this function should be called any time the number of voices the harmonizer owns changes
    void numVoicesChanged();

    /*
     */

    bool latchIsOn {false};

    juce::Array< int > currentNotes;
    juce::Array< int > desiredNotes;

    ADSRParams adsrParams;
    ADSRParams quickReleaseParams;

    float currentInputFreq {0.0f};

    double sampleRate {0.0};
    uint32 lastNoteOnCounter {0};
    int    lastPitchWheelValue {64};

    bool shouldStealNotes {true};

    synth_helpers::PanningManager panner;
    int                           lowestPannedNote {0};

    bav::midi::VelocityHelper  velocityConverter;
    bav::midi::PitchBendHelper bendTracker;

#if bvsb_USE_MTS_ESP
    MTSClient* mtsClient = nullptr;
#else
    bav::midi::PitchConverter pitchConverter;
#endif

    int lastMidiTimeStamp {0};
    int lastMidiChannel {1};

    bool aftertouchGainIsOn {true};

    float playingButReleasedMultiplier;

    bool sustainPedalDown {false};
    bool sostenutoPedalDown {false};
    bool softPedalDown {false};

    float softPedalMultiplier;  // the multiplier by which each voice's output will be multiplied when the soft pedal is down

    // *********************************

    // for clarity & cleanliness, the individual descant & pedal preferences are each encapsulated into their own struct:

    struct pedalPitchPrefs
    {
        bool isOn {false};
        int  lastPitch {-1};
        int  upperThresh {127};  // pedal pitch has an UPPER thresh - the auto harmony voice is only activated if the lowest keyboard note is BELOW a certain thresh
        int  interval {12};
    };

    struct descantPrefs
    {
        bool isOn {false};
        int  lastPitch {-1};
        int  lowerThresh {0};  // descant has a LOWER thresh - the auto harmony voice is only activated if the highest keyboard note is ABOVE a certain thresh
        int  interval {12};
    };

    pedalPitchPrefs pedal;
    descantPrefs    descant;

    // *********************************

    // this is protected, not private, to allow access for custom overrides of the addNumVoices() function.
    juce::OwnedArray< Voice > voices;


private:
    void renderVoicesInternal (juce::AudioBuffer< SampleType >& output, const int startSample, const int numSamples);


    // MIDI
    void processMidi (MidiBuffer& midiMessages);
    void handleMidiEvent (const MidiMessage& m, const int samplePosition);

    void noteOn (const int midiPitch, const float velocity, const bool isKeyboard = true, const int midiChannel = -1);
    void noteOff (const int midiNoteNumber, const float velocity, const bool allowTailOff, const bool isKeyboard = true);

    void handleAftertouch (int midiNoteNumber, int aftertouchValue);
    void handleChannelPressure (int channelPressureValue);
    void updateChannelPressure (int newIncomingAftertouch);
    void handleController (const int controllerNumber, int controllerValue);
    void handleSustainPedal (const int value);
    void handleSostenutoPedal (const int value);
    void handleSoftPedal (const int value);

    void startVoice (Voice* voice, const int midiPitch, const float velocity, const bool isKeyboard, const int midiChannel = -1);
    void stopVoice (Voice* voice, const float velocity, const bool allowTailOff);

    void turnOnList (const juce::Array< int >& toTurnOn, const float velocity, const bool partOfChord = false);
    void turnOffList (const juce::Array< int >& toTurnOff, const float velocity, const bool allowTailOff, const bool partOfChord = false);

    // this function should be called any time the collection of pitches is changed (ie, with regular keyboard input, on each note on/off, or for chord input, once after each chord is triggered). Used for things like pedal pitch, descant, etc
    void pitchCollectionChanged();

    void applyPedalPitch();
    void applyDescant();

    // voice allocation
    Voice* findFreeVoice (const bool stealIfNoneAvailable);
    Voice* findVoiceToSteal();

    Voice* getVoicePlayingNote (const int midiPitch) const;

    Voice* getCurrentDescantVoice() const;
    Voice* getCurrentPedalPitchVoice() const;


    /*==============================================================================================================
     ===============================================================================================================*/

    juce::Array< Voice* > usableVoices;  // this array is used to sort the voices when a 'steal' is requested

    int lastBlocksize;

    MidiBuffer aggregateMidiBuffer;  // this midi buffer will be used to collect the harmonizer's aggregate MIDI output

    MidiBuffer midiInputStorage;  // each block of midi that comes in is stored in here so we can refer to it later
    
    LastMovedControllerInfo lastMovedControllerInfo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthBase)
};


}  // namespace bav::dsp
