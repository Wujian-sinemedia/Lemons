
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
        SynthBase(): velocityConverter(100), pitchConverter(440, 69, 12), bendTracker(2, 2), lastBlocksize(0)
        {
            adsrParams.attack  = 0.035f;
            adsrParams.decay   = 0.06f;
            adsrParams.sustain = 0.8f;
            adsrParams.release = 0.01f;
            
            quickReleaseParams.attack  = 0.01f;
            quickReleaseParams.decay   = 0.005f;
            quickReleaseParams.sustain = 1.0f;
            quickReleaseParams.release = 0.015f;
            
            quickAttackParams.attack  = 0.015f;
            quickAttackParams.decay   = 0.01f;
            quickAttackParams.sustain = 1.0f;
            quickAttackParams.release = 0.015f;
            
            setCurrentPlaybackSampleRate (44100.0);
            
            pedal.isOn = false;
            pedal.lastPitch = -1;
            pedal.upperThresh = 0;
            pedal.interval = 12;
            
            descant.isOn = false;
            descant.lastPitch = -1;
            descant.lowerThresh = 127;
            descant.interval = 12;
            
            setConcertPitchHz (440);
        }
        
        virtual ~SynthBase()
        {
            voices.clear();
        }
        
        void initialize (const int initNumVoices, const double initSamplerate, const int initBlocksize)
        {
            jassert (initNumVoices > 0 && initSamplerate > 0 && initBlocksize > 0);
            
            changeNumVoices (initNumVoices);
            
            setCurrentPlaybackSampleRate (initSamplerate);
            
            initialized (initSamplerate, initBlocksize);
            
            prepare (initBlocksize);
        }
        
        void prepare (const int blocksize)
        {
            jassert (blocksize > 0);
            jassert (! voices.isEmpty());
            
            aggregateMidiBuffer.ensureSize (size_t(blocksize * 2));
            aggregateMidiBuffer.clear();
            
            for (auto* voice : voices)
                voice->prepare (blocksize);
            
            panner.prepare (voices.size(), false);
            lastNoteOnCounter = 0;
            
            prepared (blocksize);
        }
        
        void setCurrentPlaybackSampleRate (const double newRate)
        {
            jassert (newRate > 0);
            
            sampleRate = newRate;
            
            for (auto* voice : voices)
                voice->updateSampleRate (newRate);
            
            samplerateChanged (newRate);
        }
        
        virtual void renderVoices (juce::MidiBuffer midiMessages, juce::AudioBuffer<SampleType>& output)
        {
            jassert (! voices.isEmpty());
            jassert (sampleRate > 0);
            
            processMidi (midiMessages);
            
            output.clear();
            
            if (getNumActiveVoices() == 0)
                return;
            
            const int numSamples = output.getNumSamples();
            
            for (auto* voice : voices)
            {
                if (voice->isVoiceActive())
                    voice->renderBlock (output);
                else
                    voice->bypassedBlock (numSamples);
            }
        }
        
        void releaseResources()
        {
            aggregateMidiBuffer.clear();
            usableVoices.clear();
            currentNotes.clear();
            desiredNotes.clear();
            
            panner.releaseResources();
            
            for (auto* voice : voices)
                voice->release();
            
            release();
        }
        
        void resetRampedValues (int blocksize)
        {
            for (auto* voice : voices)
                voice->resetRampedValues (blocksize);
        }
        
        void bypassedBlock (const int numSamples, MidiBuffer& midiMessages)
        {
            processMidi (midiMessages);
            
            for (auto* voice : voices)
                voice->bypassedBlock (numSamples);
        }
        
        void processMidi (MidiBuffer& midiMessages)
        {
            aggregateMidiBuffer.clear();
            
            auto midiIterator = midiMessages.findNextSamplePosition(0);
            
            if (midiIterator == midiMessages.cend())
            {
                lastMidiTimeStamp = -1;
                return;
            }
            
            std::for_each (midiIterator,
                           midiMessages.cend(),
                           [&] (const juce::MidiMessageMetadata& meta)
                           {
                               handleMidiEvent (meta.getMessage(), meta.samplePosition);
                           } );
            
            pitchCollectionChanged();
            
            midiMessages.swapWith (aggregateMidiBuffer);
            
            lastMidiTimeStamp = -1;
        }
        
        void processMidiEvent (const MidiMessage& m)
        {
            handleMidiEvent (m, static_cast<int> (m.getTimeStamp()));
            pitchCollectionChanged();
        }
        
        void playChord (const juce::Array<int>& desiredPitches,
                        const float velocity = 1.0f,
                        const bool allowTailOffOfOld = false)
        {
            if (desiredPitches.isEmpty())
            {
                allNotesOff (allowTailOffOfOld);
                return;
            }
            
            // create array containing current pitches
            
            currentNotes.clearQuick();
            
            reportActiveNotes (currentNotes, false, true);
            
            if (currentNotes.isEmpty())
            {
                turnOnList (desiredPitches, velocity, true);
            }
            else
            {
                // 1. turn off the pitches that were previously on that are not included in the list of desired pitches
                
                desiredNotes.clearQuick();
                
                for (int note : currentNotes)
                    if (! desiredPitches.contains (note))
                        desiredNotes.add (note);
                
                turnOffList (desiredNotes, !allowTailOffOfOld, allowTailOffOfOld, true);
                
                // 2. turn on the desired pitches that aren't already on
                
                desiredNotes.clearQuick();
                
                for (int note : desiredPitches)
                    if (! currentNotes.contains (note))
                        desiredNotes.add (note);
                
                turnOnList (desiredNotes, velocity, true);
            }
        }
        
        void allNotesOff (const bool allowTailOff, const float velocity = 1.0f)
        {
            for (auto* voice : voices)
                if (voice->isVoiceActive())
                    stopVoice (voice, velocity, allowTailOff);
            
            panner.reset();
        }
        
        void turnOffAllKeyupNotes (const bool allowTailOff,
                                   const bool includePedalPitchAndDescant,
                                   const float velocity,
                                   const bool overrideSostenutoPedal)
        {
            for (auto* voice : voices)
                if ((voice->isVoiceActive() && ! voice->isKeyDown())
                    && (includePedalPitchAndDescant || (! (voice->isCurrentPedalVoice() || voice->isCurrentDescantVoice())))
                    && (overrideSostenutoPedal || ! voice->sustainingFromSostenutoPedal))
                { stopVoice (voice, velocity, allowTailOff); }
        }
        
        bool isPitchActive (const int midiPitch, const bool countRingingButReleased = false, const bool countKeyUpNotes = false) const
        {
            for (auto* voice : voices)
                if ((voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch)
                    && (countRingingButReleased || ! voice->isPlayingButReleased())
                    && (countKeyUpNotes || voice->isKeyDown()))
                { return true; }
            
            return false;
        }
        
        void reportActiveNotes (juce::Array<int>& outputArray,
                                const bool includePlayingButReleased = false,
                                const bool includeKeyUpNotes = true) const
        {
            outputArray.clearQuick();
            
            for (auto* voice : voices)
                if ((voice->isVoiceActive())
                    && (includePlayingButReleased || ! voice->isPlayingButReleased())
                    && (includeKeyUpNotes || voice->isKeyDown()))
                { outputArray.add (voice->getCurrentlyPlayingNote()); }
            
            if (! outputArray.isEmpty())
                outputArray.sort();
        }
        
        int getNumActiveVoices() const
        {
            int actives = 0;
            
            for (auto* voice : voices)
                if (voice->isVoiceActive())
                    ++actives;
            
            return actives;
        }
        
        int getNumVoices() const noexcept { return voices.size(); }
        
        void changeNumVoices (const int newNumVoices)
        {
            const int currentVoices = voices.size();
            
            if (newNumVoices > currentVoices)
                addNumVoices (newNumVoices - currentVoices);
            else if (newNumVoices < currentVoices)
                removeNumVoices (currentVoices - newNumVoices);
            
            numVoicesChanged();
            
            jassert (voices.size() == newNumVoices);
        }
        
        void setNoteStealingEnabled (const bool shouldSteal) noexcept { shouldStealNotes = shouldSteal; }
        
        void updateMidiVelocitySensitivity (int newSensitivity)
        {
            jassert (newSensitivity >= 0 && newSensitivity <= 100);
            
            const float newSens = newSensitivity / 100.0f;
            
            if (velocityConverter.getCurrentSensitivity() == newSens)
                return;
            
            velocityConverter.setFloatSensitivity (newSens);
            
            for (auto* voice : voices)
                if (voice->isVoiceActive())
                    voice->setVelocityMultiplier (getWeightedVelocity (voice->getLastRecievedVelocity()));
        }
        
        void updatePitchbendSettings (const int rangeUp, const int rangeDown)
        {
            if ((bendTracker.getCurrentRangeUp() == rangeUp) && (bendTracker.getCurrentRangeDown() == rangeDown))
                return;
            
            bendTracker.setRange (rangeUp, rangeDown);
            
            if (lastPitchWheelValue == 64)
                return;
            
            for (auto* voice : voices)
                if (voice->isVoiceActive())
                    voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
        }
        
        void setAftertouchGainOnOff (const bool shouldBeOn) { aftertouchGainIsOn = shouldBeOn; }
        
        void setPedalPitch (const bool isOn)
        {
            if (pedal.isOn == isOn)
                return;
            
            pedal.isOn = isOn;
            
            if (isOn)
                applyPedalPitch();
            else
            {
                if (pedal.lastPitch > -1)
                    noteOff (pedal.lastPitch, 1.0f, false, false);
                
                pedal.lastPitch = -1;
            }
        }
        
        void setPedalPitchUpperThresh (int newThresh)
        {
            jassert (newThresh >= 0 && newThresh <= 127);
            
            if (pedal.upperThresh == newThresh)
                return;
            
            pedal.upperThresh = newThresh;
            
            if (pedal.isOn)
                applyPedalPitch();
        }
        
        void setPedalPitchInterval (const int newInterval)
        {
            if (pedal.interval == newInterval)
                return;
            
            pedal.interval = newInterval;
            
            if (pedal.isOn)
                applyPedalPitch();
        }
        
        void setDescant (const bool isOn)
        {
            if (descant.isOn == isOn)
                return;
            
            descant.isOn = isOn;
            
            if (isOn)
                applyDescant();
            else
            {
                if (descant.lastPitch > -1)
                    noteOff (descant.lastPitch, 1.0f, false, false);
                
                descant.lastPitch = -1;
            }
        }
        
        void setDescantLowerThresh (int newThresh)
        {
            jassert (newThresh >= 0 && newThresh <= 127);
            
            if (descant.lowerThresh == newThresh)
                return;
            
            descant.lowerThresh = newThresh;
            
            if (descant.isOn)
                applyDescant();
        }
        
        void setDescantInterval (const int newInterval)
        {
            if (descant.interval == newInterval)
                return;
            
            descant.interval = newInterval;
            
            if (descant.isOn)
                applyDescant();
        }
        
        void setConcertPitchHz (const int newConcertPitchhz)
        {
            jassert (newConcertPitchhz > 0);
            
            if (pitchConverter.getCurrentConcertPitchHz() == newConcertPitchhz)
                return;
            
            pitchConverter.setConcertPitchHz (newConcertPitchhz);
            
            for (auto* voice : voices)
                if (voice->isVoiceActive())
                    voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
        }
        
        void updateStereoWidth (int newWidth)
        {
            jassert (newWidth >= 0 && newWidth <= 100);
            
            if (panner.getCurrentStereoWidth() == newWidth)
                return;
            
            panner.updateStereoWidth (newWidth);
            
            for (auto* voice : voices)
            {
                if (! voice->isVoiceActive())
                    continue;
                
                if (voice->getCurrentlyPlayingNote() < lowestPannedNote)
                {
                    const int currentPan = voice->getCurrentMidiPan();
                    
                    if (currentPan != 64)
                    {
                        panner.panValTurnedOff (currentPan);
                        voice->setPan (64);
                    }
                }
                else
                {
                    voice->setPan (panner.getClosestNewPanValFromOld (voice->getCurrentMidiPan()));
                }
            }
        }
        
        void updateLowestPannedNote (int newPitchThresh)
        {
            jassert (newPitchThresh >= 0 && newPitchThresh <= 127);
            
            const int prevLowestnote = lowestPannedNote;
            
            if (prevLowestnote == newPitchThresh)
                return;
            
            lowestPannedNote = newPitchThresh;
            
            for (auto* voice : voices)
            {
                if (! voice->isVoiceActive())
                    continue;
                
                const int note = voice->getCurrentlyPlayingNote();
                const int currentPan = voice->getCurrentMidiPan();
                
                if (note < newPitchThresh)
                {
                    if (currentPan != 64)
                    {
                        panner.panValTurnedOff (currentPan);
                        voice->setPan (64);
                    }
                }
                else if (note < prevLowestnote) // because we haven't updated the lowestPannedNote member variable yet, voices with pitches higher than newPitchThresh but lower than lowestPannedNote are the voices that now qualify for panning
                {
                    if (currentPan == 64)
                        voice->setPan (panner.getNextPanVal());
                }
            }
        }
        
        void setMidiLatch (const bool shouldBeOn, const bool allowTailOff = false)
        {
            if (latchIsOn == shouldBeOn)
                return;
            
            latchIsOn = shouldBeOn;
            
            if (shouldBeOn)
                return;
            
            turnOffAllKeyupNotes (allowTailOff, false, !allowTailOff, false);
            
            pitchCollectionChanged();
        }
        
        bool isLatched()  const noexcept { return latchIsOn; }
        
        void updateADSRsettings (const float attack, const float decay, const float sustain, const float release)
        {
            // attack/decay/release time in SECONDS; sustain ratio 0.0 - 1.0
            
            adsrParams.attack  = attack;
            adsrParams.decay   = decay;
            adsrParams.sustain = sustain;
            adsrParams.release = release;
            
            for (auto* voice : voices)
                voice->setAdsrParameters (adsrParams);
        }
        
        void setADSRonOff (const bool shouldBeOn) noexcept { adsrIsOn = shouldBeOn; }
        
        double getSamplerate() const noexcept { return sampleRate; }
        
        void handlePitchWheel (int wheelValue)
        {
            jassert (wheelValue >= 0 && wheelValue <= 127);
            
            if (lastPitchWheelValue == wheelValue)
                return;
            
            aggregateMidiBuffer.addEvent (MidiMessage::pitchWheel (lastMidiChannel, wheelValue),
                                          ++lastMidiTimeStamp);
            
            lastPitchWheelValue = wheelValue;
            bendTracker.newPitchbendRecieved (wheelValue);
            
            for (auto* voice : voices)
                if (voice->isVoiceActive())
                    voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
        }
        
        void updateQuickReleaseMs (const int newMs)
        {
            const float desiredR = newMs * 0.001f;
            
            quickReleaseParams.release = desiredR;
            quickAttackParams .release = desiredR;
            
            for (auto* voice : voices)
            {
                voice->setQuickReleaseParameters(quickReleaseParams);
                voice->setQuickAttackParameters (quickAttackParams);
            }
        }
        
        void updateQuickAttackMs  (const int newMs)
        {
            const float desiredA = newMs * 0.001f;
            
            quickAttackParams .attack = desiredA;
            quickReleaseParams.attack = desiredA;
            
            for (auto* voice : voices)
            {
                voice->setQuickAttackParameters (quickAttackParams);
                voice->setQuickReleaseParameters(quickReleaseParams);
            }
        }
        
        // returns a float velocity weighted according to the current midi velocity sensitivity settings
        float getWeightedVelocity (const float inputFloatVelocity) const { return velocityConverter.floatVelocity(inputFloatVelocity); }
        
        // returns the actual frequency in Hz a HarmonizerVoice needs to output for its latest recieved midiNote, weighted for pitchbend with the current settings & pitchwheel position, then converted to frequency with the current concert pitch settings.
        float getOutputFrequency (const int midipitch) const { return pitchConverter.mtof (bendTracker.newNoteRecieved(midipitch)); }
        
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
        void numVoicesChanged()
        {
            const int newMaxNumVoices = voices.size();
            
            panner.prepare (newMaxNumVoices, false);
            usableVoices.ensureStorageAllocated(newMaxNumVoices);
            currentNotes.ensureStorageAllocated(newMaxNumVoices);
            desiredNotes.ensureStorageAllocated(newMaxNumVoices);
        }
        
        
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
        void removeNumVoices (const int voicesToRemove)
        {
            if (voicesToRemove == 0)
                return;
            
#if JUCE_DEBUG
            const int shouldBeLeft = voices.size() - voicesToRemove;
#endif
            
            int voicesRemoved = 0;
            
            while (voicesRemoved < voicesToRemove)
            {
                if (voices.isEmpty())
                    break;
                
                Voice* removing = nullptr;
                
                for (auto* voice : voices)
                {
                    if (! voice->isVoiceActive())
                    {
                        removing = voice;
                        break;
                    }
                }
                
                if (removing == nullptr)
                    removing = findVoiceToSteal();
                
                if (removing == nullptr)
                    removing = voices[0];
                
                if (removing->isVoiceActive())
                {
                    panner.panValTurnedOff (removing->getCurrentMidiPan());
                    aggregateMidiBuffer.addEvent (MidiMessage::noteOff (lastMidiChannel, removing->getCurrentlyPlayingNote(), 1.0f),
                                                  ++lastMidiTimeStamp);
                }
                
                voices.removeObject (removing, true);
                
                ++voicesRemoved;
            }
            
            jassert (voices.isEmpty() || voices.size() == shouldBeLeft);
            
            numVoicesChanged();
        }
        
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
        void handleMidiEvent (const MidiMessage& m, const int samplePosition)
        {
            lastMidiChannel   = m.getChannel();
            lastMidiTimeStamp = samplePosition - 1;
            
            if (m.isNoteOn())
                noteOn (m.getNoteNumber(), m.getFloatVelocity(), true);
            else if (m.isNoteOff())
                noteOff (m.getNoteNumber(), m.getFloatVelocity(), true, true);
            else if (m.isAllNotesOff() || m.isAllSoundOff())
                allNotesOff (false);
            else if (m.isPitchWheel())
                handlePitchWheel (juce::jmap (m.getPitchWheelValue(), 0, 16383, 0, 127));
            else if (m.isAftertouch())
                handleAftertouch (m.getNoteNumber(), m.getAfterTouchValue());
            else if (m.isChannelPressure())
                handleChannelPressure (m.getChannelPressureValue());
            else if (m.isController())
                handleController (m.getControllerNumber(), m.getControllerValue());
        }
        
        void noteOn (const int midiPitch, const float velocity, const bool isKeyboard = true)
        {
            // N.B. the `isKeyboard` flag should be true if this note on event was triggered directly from the plugin's midi input; this flag should be false if this note event was automatically triggered by any internal function of Imogen (descant, pedal pitch, etc)
            
            Voice* voice = nullptr;
            
            if (auto* prevVoice = getVoicePlayingNote (midiPitch))
            {
                voice = prevVoice;  // retrigger the same voice with the new velocity
            }
            else
            {
                const bool isStealing = isKeyboard ? shouldStealNotes : false;  // never steal voices for automated note events, only for keyboard triggered events
                
                if (auto* newVoice = findFreeVoice (isStealing))
                {
                    voice = newVoice;
                }
                else
                {
                    if (pedal.isOn && midiPitch == pedal.lastPitch)
                        pedal.lastPitch = -1;
                    
                    if (descant.isOn && midiPitch == descant.lastPitch)
                        descant.lastPitch = -1;
                    
                    return;
                }
            }
            
            startVoice (voice, midiPitch, velocity, isKeyboard);
        }
        
        void noteOff (const int midiNoteNumber, const float velocity, const bool allowTailOff, const bool isKeyboard = true)
        {
            // N.B. the `isKeyboard` flag should be true if this note off event was triggered directly from the plugin's midi input; this flag should be false if this note event was automatically triggered by any internal function of Imogen (descant, latch, etc)
            
            auto* voice = getVoicePlayingNote (midiNoteNumber);
            
            if (voice == nullptr)
            {
                if (pedal.isOn && midiNoteNumber == pedal.lastPitch)
                    pedal.lastPitch = -1;
                
                if (descant.isOn && midiNoteNumber == descant.lastPitch)
                    descant.lastPitch = -1;
                
                return;
            }
            
            if (isKeyboard)
            {
                if (latchIsOn)
                {
                    voice->setKeyDown (false);
                }
                else
                {
                    if (! (sustainPedalDown || voice->sustainingFromSostenutoPedal))
                        stopVoice (voice, velocity, allowTailOff);
                    else
                        voice->setKeyDown (false);
                }
                
                return;
            }
            
            // this is an automated note-off event...
            
            if (! voice->isKeyDown()) // for automated note-off events, only actually stop the voice if its keyboard key isn't currently down
            {
                stopVoice (voice, velocity, allowTailOff);
                return;
            }
            
            // we're processing an automated note-off event, but the voice's keyboard key is still being held
            
            if (pedal.isOn && midiNoteNumber == pedal.lastPitch)
            {
                pedal.lastPitch = -1;
                voice->isPedalPitchVoice = false;
                voice->setKeyDown (true);  // refresh the voice's own internal tracking of its key state
            }
            
            if (descant.isOn && midiNoteNumber == descant.lastPitch)
            {
                descant.lastPitch = -1;
                voice->isDescantVoice = false;
                voice->setKeyDown (true);  // refresh the voice's own internal tracking of its key state
            }
        }
        
        void handleAftertouch (int midiNoteNumber, int aftertouchValue)
        {
            jassert (midiNoteNumber >= 0 && midiNoteNumber <= 127);
            jassert (aftertouchValue >= 0 && aftertouchValue <= 127);
            
            aggregateMidiBuffer.addEvent (MidiMessage::aftertouchChange (lastMidiChannel, midiNoteNumber, aftertouchValue),
                                          ++lastMidiTimeStamp);
            
            for (auto* voice : voices)
            {
                if (voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiNoteNumber)
                {
                    voice->aftertouchChanged (aftertouchValue);
                    break;
                }
            }
        }
        
        void handleChannelPressure (int channelPressureValue)
        {
            jassert (channelPressureValue >= 0 && channelPressureValue <= 127);
            
            aggregateMidiBuffer.addEvent (MidiMessage::channelPressureChange (lastMidiChannel, channelPressureValue),
                                          ++lastMidiTimeStamp);
            
            for (auto* voice : voices)
                voice->aftertouchChanged (channelPressureValue);
        }
        
        void updateChannelPressure (int newIncomingAftertouch)
        {
            jassert (newIncomingAftertouch >= 0 && newIncomingAftertouch <= 127);
            
            int highestAftertouch = -1;
            
            for (auto* voice : voices)
            {
                if (! voice->isVoiceActive())
                    continue;
                
                const int at = voice->currentAftertouch;
                
                if (at > highestAftertouch)
                    highestAftertouch = at;
            }
            
            if (newIncomingAftertouch < highestAftertouch)
                return;
            
            aggregateMidiBuffer.addEvent (MidiMessage::channelPressureChange (lastMidiChannel, highestAftertouch),
                                          ++lastMidiTimeStamp);
        }
        
        void handleController (const int controllerNumber, int controllerValue)
        {
            jassert (controllerValue >= 0 && controllerValue <= 127);
            
            switch (controllerNumber)
            {
                case 0x40:  handleSustainPedal    (controllerValue);    return;
                case 0x42:  handleSostenutoPedal  (controllerValue);    return;
                case 0x43:  handleSoftPedal       (controllerValue);    return;
                default:    return;
            }
        }
        
        void handleSustainPedal (const int value)
        {
            const bool isDown = (value >= 64);
            
            if (sustainPedalDown == isDown)
                return;
            
            aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (lastMidiChannel, 0x40, value),
                                          ++lastMidiTimeStamp);
            
            sustainPedalDown = isDown;
            
            if (! isDown && ! latchIsOn)
                turnOffAllKeyupNotes (false, false, 0.0f, false);
        }
        
        void handleSostenutoPedal (const int value)
        {
            const bool isDown = (value >= 64);
            
            if (sostenutoPedalDown == isDown)
                return;
            
            aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (lastMidiChannel, 0x42, value),
                                          ++lastMidiTimeStamp);
            
            sostenutoPedalDown = isDown;
            
            if (isDown && ! latchIsOn)
            {
                for (auto* voice : voices)
                    if (voice->isVoiceActive() && ! voice->isPedalPitchVoice && ! voice->isDescantVoice)
                        voice->sustainingFromSostenutoPedal = true;
            }
            else
            {
                turnOffAllKeyupNotes (false, false, 0.0f, true);
            }
        }
        
        void handleSoftPedal (const int value)
        {
            const bool isDown = value >= 64;
            
            if (softPedalDown == isDown)
                return;
            
            softPedalDown = isDown;
            
            aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (lastMidiChannel, 0x43, value),
                                          ++lastMidiTimeStamp);
            
            for (auto* voice : voices)
                voice->softPedalChanged (isDown);
        }
        
        void startVoice (Voice* voice, const int midiPitch, const float velocity, const bool isKeyboard)
        {
            if (voice == nullptr)
                return;
            
            const int prevNote = voice->getCurrentlyPlayingNote();
            const bool wasStolen = voice->isVoiceActive();  // we know the voice is being "stolen" from another note if it was already on before getting this start command
            const bool sameNoteRetriggered = (wasStolen && prevNote == midiPitch);
            
            // output midi events for this note...
            if (! sameNoteRetriggered)  // don't output any note events if the same note is being retriggered
            {
                if (wasStolen)
                    aggregateMidiBuffer.addEvent (MidiMessage::noteOff (lastMidiChannel, prevNote, 1.0f),   // voice was stolen: output a note off for the voice's previous note
                                                  ++lastMidiTimeStamp);
                
                aggregateMidiBuffer.addEvent (MidiMessage::noteOn (lastMidiChannel, midiPitch, velocity),  // output the new note on
                                              ++lastMidiTimeStamp);
            }
            else if (aftertouchGainIsOn)  // same note retriggered: output aftertouch / channel pressure
            {
                const int aftertouch = juce::jlimit (0, 127,
                                                     juce::roundToInt ((velocity - voice->getLastRecievedVelocity()) * 127.0f));
                
                if (aftertouch != voice->currentAftertouch)
                {
                    aggregateMidiBuffer.addEvent (MidiMessage::aftertouchChange (lastMidiChannel, midiPitch, aftertouch),
                                                  ++lastMidiTimeStamp);
                    
                    voice->aftertouchChanged (aftertouch);
                    
                    updateChannelPressure (aftertouch);
                }
            }
            
            if (midiPitch < lowestPannedNote)  // set pan to 64 if note is below panning threshold
            {
                if (wasStolen)
                    panner.panValTurnedOff (voice->getCurrentMidiPan());
                
                voice->setPan (64);
            }
            else if (! wasStolen)  // don't change pan if voice was stolen
            {
                voice->setPan (panner.getNextPanVal());
            }
            
            if (! wasStolen)
                voice->currentAftertouch = 0;
            
            const bool isPedal = pedal.isOn && midiPitch == pedal.lastPitch;
            const bool isDescant = descant.isOn && midiPitch == descant.lastPitch;
            
            const uint32 timestamp = sameNoteRetriggered ? voice->noteOnTime : ++lastNoteOnCounter;  // leave the timestamp the same as it was if the same note is being retriggered
            
            const bool keydown = isKeyboard ? true : voice->isKeyDown();
            
            voice->startNote (midiPitch, velocity, timestamp, keydown, isPedal, isDescant);
        }
        
        void stopVoice  (Voice* voice, const float velocity, const bool allowTailOff)
        {
            if (voice == nullptr)
                return;
            
            if (sostenutoPedalDown && voice->sustainingFromSostenutoPedal)
                return;
            
            const int note = voice->getCurrentlyPlayingNote();
            
            aggregateMidiBuffer.addEvent (MidiMessage::noteOff (lastMidiChannel, note, velocity),
                                          ++lastMidiTimeStamp);
            
            if (voice->isCurrentPedalVoice())
                pedal.lastPitch = -1;
            
            if (voice->isCurrentDescantVoice())
                descant.lastPitch = -1;
            
            voice->stopNote (velocity, allowTailOff);
        }
        
        void turnOnList  (const juce::Array<int>& toTurnOn,  const float velocity, const bool partOfChord = false)
        {
            if (toTurnOn.isEmpty())
                return;
            
            for (int note : toTurnOn)
                noteOn (note, velocity, false);
            
            if (! partOfChord)
                pitchCollectionChanged();
        }
        
        void turnOffList (const juce::Array<int>& toTurnOff, const float velocity, const bool allowTailOff, const bool partOfChord = false)
        {
            if (toTurnOff.isEmpty())
                return;
            
            for (int note : toTurnOff)
                noteOff (note, velocity, allowTailOff, false);
            
            if (! partOfChord)
                pitchCollectionChanged();
        }
        
        // this function is called any time the collection of pitches is changed (ie, with regular keyboard input, on each note on/off, or for chord input, once after each chord is triggered). Used for things like pedal pitch, descant, etc
        void pitchCollectionChanged()
        {
            if (pedal.isOn)
                applyPedalPitch();
            
            if (descant.isOn)
                applyDescant();
        }
        
        void applyPedalPitch()
        {
            if (pedal.interval == 0)
                return;
            
            int currentLowest = 128;
            Voice* lowestVoice = nullptr;
            
            for (auto* voice : voices) // find the current lowest note being played by a keyboard key
            {
                if (voice->isVoiceActive() && voice->isKeyDown())
                {
                    const int note = voice->getCurrentlyPlayingNote();
                    
                    if (note < currentLowest)
                    {
                        currentLowest = note;
                        lowestVoice = voice;
                    }
                }
            }
            
            const int lastPitch = pedal.lastPitch;
            
            if (currentLowest > pedal.upperThresh) // only create a pedal voice if the current lowest keyboard key is below a specified threshold
            {
                if (lastPitch > -1)
                    noteOff (lastPitch, 1.0f, false, false);
                
                return;
            }
            
            const int newPedalPitch = currentLowest - pedal.interval;
            
            if (newPedalPitch == lastPitch)  // pedal output note hasn't changed - do nothing
                return;
            
            if (newPedalPitch < 0 || isPitchActive (newPedalPitch, false, true))  // impossible midinote, or the new desired pedal pitch is already on
            {
                if (lastPitch > -1)
                    noteOff (lastPitch, 1.0f, false, false);
                
                return;
            }
            
            auto* prevPedalVoice = getCurrentPedalPitchVoice();  // attempt to keep the pedal line consistent - using the same HarmonizerVoice
            
            if (prevPedalVoice != nullptr)
                if (prevPedalVoice->isKeyDown())  // can't "steal" the voice playing the last pedal note if its keyboard key is down
                    prevPedalVoice = nullptr;
            
            pedal.lastPitch = newPedalPitch;
            
            if (prevPedalVoice != nullptr)
            {
                //  there was a previously active pedal voice, so steal it directly without calling noteOn:
                
                const float velocity = (lowestVoice != nullptr) ? lowestVoice->getLastRecievedVelocity() : prevPedalVoice->getLastRecievedVelocity();
                startVoice (prevPedalVoice, pedal.lastPitch, velocity, false);
            }
            else
            {
                if (lastPitch > -1)
                    noteOff (lastPitch, 1.0f, false, false);
                
                const float velocity = (lowestVoice != nullptr) ? lowestVoice->getLastRecievedVelocity() : 1.0f;
                noteOn (pedal.lastPitch, velocity, false);
            }
        }
        
        void applyDescant()
        {
            if (descant.interval == 0)
                return;
            
            int currentHighest = -1;
            Voice* highestVoice = nullptr;
            
            for (auto* voice : voices)  // find the current highest note being played by a keyboard key
            {
                if (voice->isVoiceActive() && voice->isKeyDown())
                {
                    const int note = voice->getCurrentlyPlayingNote();
                    
                    if (note > currentHighest)
                    {
                        currentHighest = note;
                        highestVoice = voice;
                    }
                }
            }
            
            const int lastPitch = descant.lastPitch;
            
            if (currentHighest < descant.lowerThresh)  // only create a descant voice if the current highest keyboard key is above a specified threshold
            {
                if (lastPitch > -1)
                    noteOff (lastPitch, 1.0f, false, false);
                
                return;
            }
            
            const int newDescantPitch = currentHighest + descant.interval;
            
            if (newDescantPitch == lastPitch)  // descant output note hasn't changed - do nothing
                return;
            
            if (newDescantPitch > 127 || isPitchActive (newDescantPitch, false, true)) // impossible midinote, or the new desired descant pitch is already on
            {
                if (lastPitch > -1)
                    noteOff (lastPitch, 1.0f, false, false);
                
                return;
            }
            
            auto* prevDescantVoice = getCurrentDescantVoice();  // attempt to keep the descant line consistent - using the same HarmonizerVoice
            
            if (prevDescantVoice != nullptr)
                if (prevDescantVoice->isKeyDown())  // can't "steal" the voice playing the last descant note if its keyboard key is down
                    prevDescantVoice = nullptr;
            
            descant.lastPitch = newDescantPitch;
            
            if (prevDescantVoice != nullptr)
            {
                //  there was a previously active descant voice, so steal it directly without calling noteOn:
                
                const float velocity = (highestVoice != nullptr) ? highestVoice->getLastRecievedVelocity() : prevDescantVoice->getLastRecievedVelocity();
                startVoice (prevDescantVoice, descant.lastPitch, velocity, false);
            }
            else
            {
                if (lastPitch > -1)
                    noteOff (lastPitch, 1.0f, false, false);
                
                const float velocity = (highestVoice != nullptr) ? highestVoice->getLastRecievedVelocity() : 1.0f;
                noteOn (descant.lastPitch, velocity, false);
            }
        }
        
        // voice allocation
        Voice* findFreeVoice (const bool stealIfNoneAvailable)
        {
            for (auto* voice : voices)
                if (! voice->isVoiceActive())
                    return voice;
            
            if (stealIfNoneAvailable)
                return findVoiceToSteal();
            
            return nullptr;
        }
        
        Voice* findVoiceToSteal()
        {
            jassert (! voices.isEmpty());
            
            // These are the voices we want to protect
            Voice* low = nullptr; // Lowest sounding note, might be sustained, but NOT in release phase
            Voice* top = nullptr; // Highest sounding note, might be sustained, but NOT in release phase
            
            // protect these, only use if necessary. These will be nullptrs if pedal / descant is currently off
            Voice* descantVoice = getCurrentDescantVoice();
            Voice* pedalVoice = getCurrentPedalPitchVoice();
            
            usableVoices.clearQuick(); // this is a list of voices we can steal, sorted by how long they've been on
            
            for (auto* voice : voices)
            {
                if (voice == descantVoice || voice == pedalVoice)
                    continue;
                
                usableVoices.add (voice);
                
                // NB: Using a functor rather than a lambda here due to scare-stories about compilers generating code containing heap allocations...
                struct Sorter
                {
                    bool operator() (const Voice* a, const Voice* b) const noexcept { return a->wasStartedBefore (*b); }
                };
                
                std::sort (usableVoices.begin(), usableVoices.end(), Sorter());
                
                if (voice->isVoiceActive() && ! voice->isPlayingButReleased())
                {
                    auto note = voice->getCurrentlyPlayingNote();
                    
                    if (low == nullptr || note < low->getCurrentlyPlayingNote())
                        low = voice;
                    
                    if (top == nullptr || note > top->getCurrentlyPlayingNote())
                        top = voice;
                }
            }
            
            if (top == low)  // Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
                top = nullptr;
            
            for (auto* voice : usableVoices)
                if (voice != low && voice != top && ! voice->isKeyDown())
                    return voice;
            
            for (auto* voice : usableVoices)
                if (voice != low && voice != top)
                    return voice;
            
            // only protected top & bottom voices are left now - time to use the pedal pitch & descant voices...
            
            if (descantVoice != nullptr) // save bass
                return descantVoice;
            
            if (pedalVoice != nullptr)
                return pedalVoice;
            
            // return final top & bottom notes held with keyboard keys
            
            if (top != nullptr) // save bass
                return top;
            
            return low;
        }
        
        Voice* getVoicePlayingNote (const int midiPitch) const
        {
            for (auto* voice : voices)
                if (voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch)
                    return voice;
            
            return nullptr;
        }
        
        Voice* getCurrentDescantVoice() const
        {
            if (! descant.isOn)
                return nullptr;
            
            for (auto* voice : voices)
                if (voice->isVoiceActive() && voice->isCurrentDescantVoice())
                    return voice;
            
            return nullptr;
        }
        
        Voice* getCurrentPedalPitchVoice() const
        {
            if (! pedal.isOn)
                return nullptr;
            
            for (auto* voice : voices)
                if (voice->isVoiceActive() && voice->isCurrentPedalVoice())
                    return voice;
            
            return nullptr;
        }
        
        
        /*==============================================================================================================
         ===============================================================================================================*/
        
        juce::Array< Voice* > usableVoices; // this array is used to sort the voices when a 'steal' is requested
        
        int lastBlocksize;
        
        MidiBuffer aggregateMidiBuffer; // this midi buffer will be used to collect the harmonizer's aggregate MIDI output
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthBase)
    };
    
    
    template class SynthBase<float>;
    template class SynthBase<double>;
    
    
}  // namespace




