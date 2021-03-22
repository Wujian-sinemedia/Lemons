
namespace bav::dsp
{
    
    template<typename SampleType>
    SynthBase<SampleType>::SynthBase(): velocityConverter(100), pitchConverter(440, 69, 12), bendTracker(2, 2), lastBlocksize(0)
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
    
    
    template<typename SampleType>
    SynthBase<SampleType>::~SynthBase()
    {
        voices.clear();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::initialize (const int initNumVoices, const double initSamplerate, const int initBlocksize)
    {
        jassert (initNumVoices > 0 && initSamplerate > 0 && initBlocksize > 0);
        
        changeNumVoices (initNumVoices);
        
        setCurrentPlaybackSampleRate (initSamplerate);
        
        initialized (initSamplerate, initBlocksize);
        
        prepare (initBlocksize);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::prepare (const int blocksize)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setCurrentPlaybackSampleRate (const double newRate)
    {
        jassert (newRate > 0);
        
        sampleRate = newRate;
        
        for (auto* voice : voices)
            voice->updateSampleRate (newRate);
        
        samplerateChanged (newRate);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::renderVoices (juce::MidiBuffer midiMessages, juce::AudioBuffer<SampleType>& output)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::bypassedBlock (const int numSamples, MidiBuffer& midiMessages)
    {
        processMidi (midiMessages);
        
        for (auto* voice : voices)
            voice->bypassedBlock (numSamples);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::releaseResources()
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::resetRampedValues (int blocksize)
    {
        for (auto* voice : voices)
            voice->resetRampedValues (blocksize);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::processMidi (MidiBuffer& midiMessages)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::processMidiEvent (const MidiMessage& m)
    {
        handleMidiEvent (m, static_cast<int> (m.getTimeStamp()));
        pitchCollectionChanged();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::playChord (const juce::Array<int>& desiredPitches,
                                           const float velocity = 1.0f,
                                           const bool allowTailOffOfOld)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::allNotesOff (const bool allowTailOff, const float velocity)
    {
        for (auto* voice : voices)
            if (voice->isVoiceActive())
                stopVoice (voice, velocity, allowTailOff);
        
        panner.reset();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::turnOffAllKeyupNotes (const bool allowTailOff,
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
    
    
    template<typename SampleType>
    bool SynthBase<SampleType>::isPitchActive (const int midiPitch, const bool countRingingButReleased, const bool countKeyUpNotes) const
    {
        for (auto* voice : voices)
            if ((voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch)
                && (countRingingButReleased || ! voice->isPlayingButReleased())
                && (countKeyUpNotes || voice->isKeyDown()))
            { return true; }
        
        return false;
    }
    
    
    template<typename SampleType>
    bool SynthBase<SampleType>::reportActiveNotes (juce::Array<int>& outputArray,
                                                   const bool includePlayingButReleased,
                                                   const bool includeKeyUpNotes) const
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
    
    
    template<typename SampleType>
    int SynthBase<SampleType>::getNumActiveVoices() const
    {
        int actives = 0;
        
        for (auto* voice : voices)
            if (voice->isVoiceActive())
                ++actives;
        
        return actives;
    }
    
    
    template<typename SampleType>
    int SynthBase<SampleType>::getNumVoices() const noexcept { return voices.size(); }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::changeNumVoices (const int newNumVoices)
    {
        const int currentVoices = voices.size();
        
        if (newNumVoices > currentVoices)
            addNumVoices (newNumVoices - currentVoices);
        else if (newNumVoices < currentVoices)
            removeNumVoices (currentVoices - newNumVoices);
        
        numVoicesChanged();
        
        jassert (voices.size() == newNumVoices);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updateMidiVelocitySensitivity (int newSensitivity)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updatePitchbendSettings (const int rangeUp, const int rangeDown)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setPedalPitch (const bool isOn)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setPedalPitchUpperThresh (int newThresh)
    {
        jassert (newThresh >= 0 && newThresh <= 127);
        
        if (pedal.upperThresh == newThresh)
            return;
        
        pedal.upperThresh = newThresh;
        
        if (pedal.isOn)
            applyPedalPitch();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setPedalPitchInterval (const int newInterval)
    {
        if (pedal.interval == newInterval)
            return;
        
        pedal.interval = newInterval;
        
        if (pedal.isOn)
            applyPedalPitch();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setDescant (const bool isOn)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setDescantLowerThresh (int newThresh)
    {
        jassert (newThresh >= 0 && newThresh <= 127);
        
        if (descant.lowerThresh == newThresh)
            return;
        
        descant.lowerThresh = newThresh;
        
        if (descant.isOn)
            applyDescant();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setDescantInterval (const int newInterval)
    {
        if (descant.interval == newInterval)
            return;
        
        descant.interval = newInterval;
        
        if (descant.isOn)
            applyDescant();
    }
    
    template<typename SampleType>
    void SynthBase<SampleType>::setConcertPitchHz (const int newConcertPitchhz)
    {
        jassert (newConcertPitchhz > 0);
        
        if (pitchConverter.getCurrentConcertPitchHz() == newConcertPitchhz)
            return;
        
        pitchConverter.setConcertPitchHz (newConcertPitchhz);
        
        for (auto* voice : voices)
            if (voice->isVoiceActive())
                voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updateStereoWidth (int newWidth)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updateLowestPannedNote (int newPitchThresh)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::setMidiLatch (const bool shouldBeOn, const bool allowTailOff)
    {
        if (latchIsOn == shouldBeOn)
            return;
        
        latchIsOn = shouldBeOn;
        
        if (shouldBeOn)
            return;
        
        turnOffAllKeyupNotes (allowTailOff, false, !allowTailOff, false);
        
        pitchCollectionChanged();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updateADSRsettings (const float attack, const float decay, const float sustain, const float release)
    {
        // attack/decay/release time in SECONDS; sustain ratio 0.0 - 1.0
        
        adsrParams.attack  = attack;
        adsrParams.decay   = decay;
        adsrParams.sustain = sustain;
        adsrParams.release = release;
        
        for (auto* voice : voices)
            voice->setAdsrParameters (adsrParams);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handlePitchWheel (int wheelValue)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updateQuickReleaseMs (const int newMs)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updateQuickAttackMs  (const int newMs)
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
    
    
    template<typename SampleType>
    float SynthBase<SampleType>::getWeightedVelocity (const float inputFloatVelocity) const
    {
        return velocityConverter.floatVelocity(inputFloatVelocity);
    }
    
    
    template<typename SampleType>
    float SynthBase<SampleType>::getOutputFrequency (const int midipitch) const
    {
        return pitchConverter.mtof (bendTracker.newNoteRecieved(midipitch));
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::numVoicesChanged()
    {
        const int newMaxNumVoices = voices.size();
        
        panner.prepare (newMaxNumVoices, false);
        usableVoices.ensureStorageAllocated(newMaxNumVoices);
        currentNotes.ensureStorageAllocated(newMaxNumVoices);
        desiredNotes.ensureStorageAllocated(newMaxNumVoices);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::removeNumVoices (const int voicesToRemove)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handleMidiEvent (const MidiMessage& m, const int samplePosition)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::noteOn (const int midiPitch, const float velocity, const bool isKeyboard)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::noteOff (const int midiNoteNumber, const float velocity, const bool allowTailOff, const bool isKeyboard)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handleAftertouch (int midiNoteNumber, int aftertouchValue)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handleChannelPressure (int channelPressureValue)
    {
        jassert (channelPressureValue >= 0 && channelPressureValue <= 127);
        
        aggregateMidiBuffer.addEvent (MidiMessage::channelPressureChange (lastMidiChannel, channelPressureValue),
                                      ++lastMidiTimeStamp);
        
        for (auto* voice : voices)
            voice->aftertouchChanged (channelPressureValue);
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::updateChannelPressure (int newIncomingAftertouch)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handleController (const int controllerNumber, int controllerValue)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handleSustainPedal (const int value)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handleSostenutoPedal (const int value)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::handleSoftPedal (const int value)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::startVoice (Voice* voice, const int midiPitch, const float velocity, const bool isKeyboard)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::stopVoice  (Voice* voice, const float velocity, const bool allowTailOff)
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::turnOnList  (const juce::Array<int>& toTurnOn,  const float velocity, const bool partOfChord)
    {
        if (toTurnOn.isEmpty())
            return;
        
        for (int note : toTurnOn)
            noteOn (note, velocity, false);
        
        if (! partOfChord)
            pitchCollectionChanged();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::turnOffList (const juce::Array<int>& toTurnOff,
                                             const float velocity, const bool allowTailOff, const bool partOfChord)
    {
        if (toTurnOff.isEmpty())
            return;
        
        for (int note : toTurnOff)
            noteOff (note, velocity, allowTailOff, false);
        
        if (! partOfChord)
            pitchCollectionChanged();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::pitchCollectionChanged()
    {
        if (pedal.isOn)
            applyPedalPitch();
        
        if (descant.isOn)
            applyDescant();
    }
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::applyPedalPitch()
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
    
    
    template<typename SampleType>
    void SynthBase<SampleType>::applyDescant()
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
    
    
    template<typename SampleType>
    SynthVoice<SampleType>* SynthBase<SampleType>::findFreeVoice (const bool stealIfNoneAvailable)
    {
        for (auto* voice : voices)
            if (! voice->isVoiceActive())
                return voice;
        
        if (stealIfNoneAvailable)
            return findVoiceToSteal();
        
        return nullptr;
    }
    
    
    template<typename SampleType>
    SynthVoice<SampleType>* SynthBase<SampleType>::findVoiceToSteal()
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
    
    
    template<typename SampleType>
    SynthVoice<SampleType>* SynthBase<SampleType>::getVoicePlayingNote (const int midiPitch) const
    {
        for (auto* voice : voices)
            if (voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch)
                return voice;
        
        return nullptr;
    }
    
    
    template<typename SampleType>
    SynthVoice<SampleType>* SynthBase<SampleType>::getCurrentDescantVoice() const
    {
        if (! descant.isOn)
            return nullptr;
        
        for (auto* voice : voices)
            if (voice->isVoiceActive() && voice->isCurrentDescantVoice())
                return voice;
        
        return nullptr;
    }
    
    
    template<typename SampleType>
    SynthVoice<SampleType>* SynthBase<SampleType>::getCurrentPedalPitchVoice() const
    {
        if (! pedal.isOn)
            return nullptr;
        
        for (auto* voice : voices)
            if (voice->isVoiceActive() && voice->isCurrentPedalVoice())
                return voice;
        
        return nullptr;
    }
    
    
    
}  // namespace
