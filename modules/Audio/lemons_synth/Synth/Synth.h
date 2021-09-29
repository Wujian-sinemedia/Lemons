
#pragma once

#include "helpers/AutomatedHarmonyVoice.h"
#include "helpers/PanningManager.h"
#include "helpers/MidiManager.h"
#include "helpers/VoiceAllocator.h"


namespace lemons::dsp
{
template < typename SampleType >
class SynthBase
{
    using uint32     = juce::uint32;
    using Voice      = SynthVoiceBase< SampleType >;
    using ADSRParams = juce::ADSR::Parameters;

public:
    virtual ~SynthBase() = default;

    void initialize (int initNumVoices, double initSamplerate = 44100.0, int initBlocksize = 512);
    bool isInitialized() const;

    void prepare (double samplerate, int blocksize);

    void releaseResources();
    void reset();

    void renderVoices (MidiBuffer& midiMessages, AudioBuffer< SampleType >& output);

    void bypassedBlock (int numSamples, MidiBuffer& midiMessages);

    void processMidiEvent (const MidiMessage& m);

    void playChord (const juce::Array< int >& desiredPitches, float velocity = 1.0f, bool allowTailOffOfOld = false);

    void allNotesOff (bool allowTailOff = false, float velocity = 1.0f);

    void turnOffAllKeyupNotes (bool  allowTailOff                = false,
                               bool  includePedalPitchAndDescant = true,
                               float velocity                    = 1.0f,
                               bool  overrideSostenutoPedal      = true);

    bool isPitchActive (int midiPitch, bool countRingingButReleased = false, bool countKeyUpNotes = false) const;
    void reportActiveNotes (juce::Array< int >& outputArray, bool includePlayingButReleased = false, bool includeKeyUpNotes = true) const;

    int  getNumActiveVoices() const;
    int  getNumVoices() const noexcept { return voices.size(); }
    void changeNumVoices (int newNumVoices);

    void setNoteStealingEnabled (bool shouldSteal) noexcept { shouldStealNotes = shouldSteal; }
    void updateMidiVelocitySensitivity (int newSensitivity);
    void updatePitchbendRange (int rangeUp, int rangeDown);
    void updatePitchbendRange (int rangeSemitones);
    void setAftertouchGainOnOff (bool shouldBeOn) { aftertouchGainIsOn = shouldBeOn; }

    void setMidiLatch (bool shouldBeOn, const bool allowTailOff = false);
    bool isLatched() const noexcept { return latchIsOn; }

    void updateADSRsettings (float attack, float decay, float sustain, float release);
    void updateQuickReleaseMs (int newMs);

    bool isSustainPedalDown() const noexcept { return midi.router.isSustainPedalDown(); }
    bool isSostenutoPedalDown() const noexcept { return midi.router.isSostenutoPedalDown(); }
    bool isSoftPedalDown() const noexcept { return midi.router.isSoftPedalDown(); }
    bool isAftertouchGainOn() const noexcept { return aftertouchGainIsOn; }

    bool   isConnectedToMtsEsp() const { return pitch.tuning.isConnectedToMtsEsp(); }
    String getScaleName() const { return pitch.tuning.getScaleName(); }

    auto getLastMovedControllerInfo() const { return midi.router.getLastMovedCCinfo(); }

    void setPitchGlideTime (double glideTimeSeconds);
    void togglePitchGlide (bool shouldGlide);

    const midi::PitchPipeline* getPitchAdjuster() { return &pitch; }

    juce::MidiKeyboardState keyboardState;

    synth::PanningManager< SampleType > panner {*this};

    synth::AutomatedHarmonyVoice< SampleType > pedal {*this, false};
    synth::AutomatedHarmonyVoice< SampleType > descant {*this, true};

    /*-----------------------------------------------------*/

    struct TimbreModParams
    {
        FX::FilterParams filterParams {FX::FilterType::Notch,
                                       2600.f, 1.3f, 1.7f};

        float gain {1.f};

        bool filterToggle {false};
    };

    TimbreModParams playingButReleased, softPedal;

    /*-----------------------------------------------------*/

protected:
    friend class SynthVoiceBase< SampleType >;
    friend class synth::AutomatedHarmonyVoice< SampleType >;
    friend class synth::PanningManager< SampleType >;
    friend class synth::MidiManager< SampleType >;
    friend class synth::VoiceAllocator< SampleType >;

    virtual void initialized (double initSamplerate, int initBlocksize) { juce::ignoreUnused (initSamplerate, initBlocksize); }

    virtual void prepared (double samplerate, int blocksize) { juce::ignoreUnused (samplerate, blocksize); }

    virtual void resetTriggered() { }

    virtual void release() { }

    // this method should return an instance of your synth's voice subclass
    virtual Voice* createVoice() = 0;

private:
    void addNumVoices (int voicesToAdd);
    void removeNumVoices (int voicesToRemove);
    void numVoicesChanged();

    void noteOn (int midiPitch, float velocity, bool isKeyboard = true, int midiChannel = -1);
    void noteOff (int midiNoteNumber, float velocity = 1.0f, bool allowTailOff = false, bool isKeyboard = true);
    void startVoice (Voice* voice, int midiPitch, float velocity, bool isKeyboard, int midiChannel = -1);
    void stopVoice (Voice* voice, float velocity, bool allowTailOff);
    void turnOnList (const juce::Array< int >& toTurnOn, float velocity = 1.0f, bool partOfChord = false);
    void turnOffList (const juce::Array< int >& toTurnOff, float velocity = 1.0f, bool allowTailOff = false, bool partOfChord = false);

    void pitchCollectionChanged();

    void updateChannelPressure (int newIncomingAftertouch);

    Voice* getVoicePlayingNote (int midiPitch) const;

    /*==============================================================================================================
     ===============================================================================================================*/

    OwnedArray< Voice > voices;

    bool latchIsOn {false}, shouldStealNotes {true}, aftertouchGainIsOn {true};

    Array< int > currentNotes, desiredNotes;

    ADSRParams adsrParams {0.035f, 0.06f, 0.8f, 0.01f};
    ADSRParams quickReleaseParams {0.01f, 0.005f, 1.0f, 0.015f};

    double sampleRate {0.0};

    midi::VelocityHelper velocityConverter;
    midi::PitchPipeline  pitch;

    int lastBlocksize {0};

    MidiBuffer aggregateMidiBuffer, midiInputStorage;

    synth::MidiManager< SampleType > midi {*this};

    synth::VoiceAllocator< SampleType > voiceAllocator {*this};
};


}  // namespace bav::dsp
