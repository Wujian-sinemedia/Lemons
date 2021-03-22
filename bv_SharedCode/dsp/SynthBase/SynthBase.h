
/*
    Base class for a polyphonic synthesiser-style instrument.
    This class owns, manages & manipulates a collection of SynthVoiceBase (or SynthVoiceBase-derived) objects.
*/


#include "PanningManager.h"


namespace bav::dsp
{

template<typename SampleType>
class SynthBase
{
    using AudioBuffer = juce::AudioBuffer<SampleType>;
    using MidiBuffer  = juce::MidiBuffer;
    using MidiMessage = juce::MidiMessage;
    
    using Voice = SynthVoiceBase<SampleType>;
    
    using ADSRParams = juce::ADSR::Parameters;
    
    
public:
    SynthBase();
    
    virtual ~SynthBase();
    
    void initialize (const int initNumVoices, const double initSamplerate, const int initBlocksize);
    
    void prepare (const int blocksize);
    
    void setCurrentPlaybackSampleRate (const double newRate);
    
    virtual void renderVoices (juce::MidiBuffer midiMessages, juce::AudioBuffer<SampleType>& output);
    
    void releaseResources();
    
    void resetRampedValues (int blocksize);
    
    void bypassedBlock (const int numSamples, MidiBuffer& midiMessages);
    
    void processMidi (MidiBuffer& midiMessages);
    
    void processMidiEvent (const MidiMessage& m);
    
    void playChord (const juce::Array<int>& desiredPitches,
                    const float velocity = 1.0f,
                    const bool allowTailOffOfOld = false);
    
    void allNotesOff (const bool allowTailOff, const float velocity = 1.0f);
    
    void turnOffAllKeyupNotes (const bool allowTailOff,
                               const bool includePedalPitchAndDescant,
                               const float velocity,
                               const bool overrideSostenutoPedal);
    
    bool isPitchActive (const int midiPitch, const bool countRingingButReleased = false, const bool countKeyUpNotes = false) const;
    
    void reportActiveNotes (juce::Array<int>& outputArray, const bool includePlayingButReleased = false, const bool includeKeyUpNotes = true) const;
    
    int getNumActiveVoices() const;
    
    int getNumVoices() const noexcept;
    
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
    
    void setConcertPitchHz (const int newConcertPitchhz);
    
    void updateStereoWidth (int newWidth);
    
    void updateLowestPannedNote (int newPitchThresh);
    
    void setMidiLatch (const bool shouldBeOn, const bool allowTailOff = false);
    
    bool isLatched()  const noexcept { return latchIsOn; }
    
    void updateADSRsettings (const float attack, const float decay, const float sustain, const float release);
    
    void setADSRonOff (const bool shouldBeOn) noexcept { adsrIsOn = shouldBeOn; }
    
    double getSamplerate() const noexcept { return sampleRate; }
    
    void handlePitchWheel (int wheelValue);
    
    void updateQuickReleaseMs (const int newMs);
    
    void updateQuickAttackMs  (const int newMs);
    
    // returns a float velocity weighted according to the current midi velocity sensitivity settings
    float getWeightedVelocity (const float inputFloatVelocity) const;
    
    // returns the actual frequency in Hz a HarmonizerVoice needs to output for its latest recieved midiNote, weighted for pitchbend with the current settings & pitchwheel position, then converted to frequency with the current concert pitch settings.
    float getOutputFrequency (const int midipitch) const;
    
    bool isSustainPedalDown()   const noexcept { return sustainPedalDown;   }
    bool isSostenutoPedalDown() const noexcept { return sostenutoPedalDown; }
    bool isSoftPedalDown()      const noexcept { return softPedalDown;      }
    float getSoftPedalMultiplier() const noexcept { return softPedalMultiplier; }
    float getPlayingButReleasedMultiplier() const noexcept { return playingButReleasedMultiplier; }
    bool isAftertouchGainOn() const noexcept { return aftertouchGainIsOn; }
    
    bool isADSRon() const noexcept { return adsrIsOn; }
    ADSRParams getCurrentAdsrParams() const noexcept { return adsrParams; }
    ADSRParams getCurrentQuickReleaseParams() const noexcept { return quickReleaseParams; }
    ADSRParams getCurrentQuickAttackParams()  const noexcept { return quickAttackParams; }
    
    int getLastBlocksize() const noexcept { return lastBlocksize; }
    
    

protected:
    friend class SynthVoiceBase<SampleType>;
    
    juce::OwnedArray< Voice > voices;
    
    bool latchIsOn;
    
    juce::Array<int> currentNotes;
    juce::Array<int> desiredNotes;
    
    ADSRParams adsrParams;
    ADSRParams quickReleaseParams;
    ADSRParams quickAttackParams;
    
    float currentInputFreq;
    
    double sampleRate;
    uint32 lastNoteOnCounter;
    int lastPitchWheelValue;
    
    bool shouldStealNotes;
    
    PanningManager  panner;
    int lowestPannedNote;
    
    bav::midi::VelocityHelper  velocityConverter;
    bav::midi::PitchConverter  pitchConverter;
    bav::midi::PitchBendHelper bendTracker;
    
    bool adsrIsOn;
    
    int lastMidiTimeStamp;
    int lastMidiChannel;
    
    bool aftertouchGainIsOn;
    
    float playingButReleasedMultiplier;
    
    bool sustainPedalDown, sostenutoPedalDown, softPedalDown;
    
    float softPedalMultiplier; // the multiplier by which each voice's output will be multiplied when the soft pedal is down
    
    // *********************************
    
    // for clarity & cleanliness, the individual descant & pedal preferences are each encapsulated into their own struct:
    
    struct pedalPitchPrefs
    {
        bool isOn;
        int lastPitch;
        int upperThresh; // pedal pitch has an UPPER thresh - the auto harmony voice is only activated if the lowest keyboard note is BELOW a certain thresh
        int interval;
    };
    
    struct descantPrefs
    {
        bool isOn;
        int lastPitch;
        int lowerThresh; // descant has a LOWER thresh - the auto harmony voice is only activated if the highest keyboard note is ABOVE a certain thresh
        int interval;
    };
    
    pedalPitchPrefs pedal;
    descantPrefs descant;
    
    // *********************************
    
    // this function should be called any time the number of voices the harmonizer owns changes
    void numVoicesChanged();
    
    
private:
    
    // if overridden, called in the subclass when the top-level call to initialize() is made.
    virtual void initialized (const double initSamplerate, const int initBlocksize)
    {
        juce::ignoreUnused (initSamplerate, initBlocksize);
    }
    
    // if overridden, called in the subclass when the top-level call to prepare() is made.
    virtual void prepared (int blocksize) { juce::ignoreUnused (blocksize); }
    
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
    virtual void addNumVoices (const int voicesToAdd)
    {
        if (voicesToAdd == 0)
            return;
        
        for (int i = 0; i < voicesToAdd; ++i)
            voices.add (new Voice(this, sampleRate));
        
        jassert (voices.size() >= voicesToAdd);
        
        numVoicesChanged();
    }
    
    
    // MIDI
    void handleMidiEvent (const MidiMessage& m, const int samplePosition);
    
    void noteOn (const int midiPitch, const float velocity, const bool isKeyboard = true);
    
    void noteOff (const int midiNoteNumber, const float velocity, const bool allowTailOff, const bool isKeyboard = true);
    
    void handleAftertouch (int midiNoteNumber, int aftertouchValue);
    
    void handleChannelPressure (int channelPressureValue);
    
    void updateChannelPressure (int newIncomingAftertouch);
    
    void handleController (const int controllerNumber, int controllerValue);
    
    void handleSustainPedal (const int value);
    
    void handleSostenutoPedal (const int value);
    
    void handleSoftPedal (const int value);
    
    void startVoice (Voice* voice, const int midiPitch, const float velocity, const bool isKeyboard);
    
    void stopVoice  (Voice* voice, const float velocity, const bool allowTailOff);
    
    void turnOnList  (const juce::Array<int>& toTurnOn,  const float velocity, const bool partOfChord = false);
    
    void turnOffList (const juce::Array<int>& toTurnOff, const float velocity, const bool allowTailOff, const bool partOfChord = false);
    
    // this function is called any time the collection of pitches is changed (ie, with regular keyboard input, on each note on/off, or for chord input, once after each chord is triggered). Used for things like pedal pitch, descant, etc
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
    
    juce::Array< Voice* > usableVoices; // this array is used to sort the voices when a 'steal' is requested
    
    int lastBlocksize;
    
    MidiBuffer aggregateMidiBuffer; // this midi buffer will be used to collect the harmonizer's aggregate MIDI output
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthBase)
};


}  // namespace



