
#include "PanningManager/PanningManager.cpp"


namespace bav::dsp
{
/*
 Constructor.
 */
template < typename SampleType >
SynthBase< SampleType >::SynthBase()
    : velocityConverter (100), bendTracker (2, 2)
#if ! bvsb_USE_MTS_ESP
      , pitchConverter (440, 69, 12)
#endif
      , lastBlocksize (0)
{
    adsrParams.attack  = 0.035f;
    adsrParams.decay   = 0.06f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.01f;

    quickReleaseParams.attack  = 0.01f;
    quickReleaseParams.decay   = 0.005f;
    quickReleaseParams.sustain = 1.0f;
    quickReleaseParams.release = 0.015f;

    setCurrentPlaybackSampleRate (44100.0);

    pedal.isOn        = false;
    pedal.lastPitch   = -1;
    pedal.upperThresh = 0;
    pedal.interval    = 12;

    descant.isOn        = false;
    descant.lastPitch   = -1;
    descant.lowerThresh = 127;
    descant.interval    = 12;

    setConcertPitchHz (440);

#if bvsb_USE_MTS_ESP
    mtsClient = MTS_RegisterClient();
    jassert (mtsClient != nullptr);
#endif
}


/*
 Destructor.
 */
template < typename SampleType >
SynthBase< SampleType >::~SynthBase()
{
    voices.clear();

#if bvsb_USE_MTS_ESP
    MTS_DeregisterClient (mtsClient);
#endif
}


/*
 Initializes the synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::initialize (const int initNumVoices, const double initSamplerate, const int initBlocksize)
{
    jassert (initNumVoices > 0 && initSamplerate > 0 && initBlocksize > 0);

    changeNumVoices (initNumVoices);

    setCurrentPlaybackSampleRate (initSamplerate);

    initialized (initSamplerate, initBlocksize);

    prepare (initBlocksize);
}


/*
 Immediately clears all notes & resets the synth's internal state.
 */
template < typename SampleType >
void SynthBase< SampleType >::reset()
{
    for (auto* voice : voices)
        voice->clearCurrentNote();

    panner.reset();

    resetTriggered();
}


/*
 Prepares the synth for a new expected maximum blocksize.
 */
template < typename SampleType >
void SynthBase< SampleType >::prepare (const int blocksize)
{
    jassert (blocksize > 0);
    jassert (! voices.isEmpty());

    aggregateMidiBuffer.ensureSize (size_t (blocksize * 2));
    aggregateMidiBuffer.clear();

    for (auto* voice : voices)
        voice->prepare (blocksize);

    panner.prepare (voices.size(), false);
    lastNoteOnCounter = 0;

    resetRampedValues (blocksize);

    prepared (blocksize);
}


/*
 Attempts to reduce the synth's footprint as much as possible.
 After calling this, you must call initialize() again before attempting to render any more audio with the synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::releaseResources()
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


/*=========================================================================================================
          _    _ _____ _____ ____     _____  ______ _   _ _____  ______ _____  _____ _   _  _____
     /\  | |  | |  __ \_   _/ __ \   |  __ \|  ____| \ | |  __ \|  ____|  __ \|_   _| \ | |/ ____|
    /  \ | |  | | |  | || || |  | |  | |__) | |__  |  \| | |  | | |__  | |__) | | | |  \| | |  __
   / /\ \| |  | | |  | || || |  | |  |  _  /|  __| | . ` | |  | |  __| |  _  /  | | | . ` | | |_ |
  / ____ \ |__| | |__| || || |__| |  | | \ \| |____| |\  | |__| | |____| | \ \ _| |_| |\  | |__| |
 /_/    \_\____/|_____/_____\____/   |_|  \_\______|_| \_|_____/|______|_|  \_\_____|_| \_|\_____|
 
 Functions for audio rendering
 
 ==========================================================================================================*/

/*
 Renders the synth's composite stereo output to "output". (If a mono buffer is passed, the synth will return mono output.)
 Reads events from and returns the synth's composite midi output to "midiMessages". Note that due to the midi latch, pedal pitch, and descant features, the returned midi buffer may have more or fewer events than the original passed midi buffer.
 Internally, the synth will render smaller chunks of audio in between each midi message timestamp, to keep midi events synchronous with the audio output. If you are implementing a class that derives from this class or the SynthVoiceBase class, note that the SynthVoiceBase's renderPlease() method may recieve blocksizes as small as 1 sample (but never 0 samples).
 */
template < typename SampleType >
void SynthBase< SampleType >::renderVoices (juce::MidiBuffer& midiMessages, juce::AudioBuffer< SampleType >& output)
{
    jassert (! voices.isEmpty());
    jassert (sampleRate > 0);

    output.clear();

    aggregateMidiBuffer.clear();
    lastMidiTimeStamp = -1;

    auto samplesLeft = output.getNumSamples();

    midiInputStorage.clear();
    midiInputStorage.addEvents (midiMessages, 0, samplesLeft, 0);

    for (auto* voice : voices)
        voice->newBlockComing (lastBlocksize, samplesLeft);

    lastBlocksize = samplesLeft;

    auto midiIterator = midiMessages.findNextSamplePosition (0);

    int startSample = 0;

    for (; samplesLeft > 0; ++midiIterator)
    {
        if (midiIterator == midiMessages.cend())
        {
            renderVoicesInternal (output, startSample, samplesLeft);
            midiMessages.swapWith (aggregateMidiBuffer);
            lastMidiTimeStamp = -1;
            midiInputStorage.clear();
            return;
        }

        const auto metadata                 = *midiIterator;
        const auto samplesToNextMidiMessage = metadata.samplePosition - startSample;

        if (samplesToNextMidiMessage >= samplesLeft)
        {
            renderVoicesInternal (output, startSample, samplesLeft);
            handleMidiEvent (metadata.getMessage(), metadata.samplePosition);
            break;
        }

        if (samplesToNextMidiMessage == 0)
        {
            handleMidiEvent (metadata.getMessage(), metadata.samplePosition);
            continue;
        }

        renderVoicesInternal (output, startSample, samplesToNextMidiMessage);
        handleMidiEvent (metadata.getMessage(), metadata.samplePosition);

        startSample += samplesToNextMidiMessage;
        samplesLeft -= samplesToNextMidiMessage;
    }

    std::for_each (
        midiIterator, midiMessages.cend(), [&] (const juce::MidiMessageMetadata& meta)
        { handleMidiEvent (meta.getMessage(), meta.samplePosition); });

    midiMessages.swapWith (aggregateMidiBuffer);
    lastMidiTimeStamp = -1;
    midiInputStorage.clear();
}


/*
 Renders all the synth's voices for the given range of the output buffer. (Not for public use.)
 */
template < typename SampleType >
void SynthBase< SampleType >::renderVoicesInternal (juce::AudioBuffer< SampleType >& output, const int startSample, const int numSamples)
{
#if JUCE_DEBUG
    const auto totalNumSamples = output.getNumSamples();
    jassert (numSamples <= totalNumSamples && startSample < totalNumSamples);
#endif
    for (auto* voice : voices)
    {
        if (voice->isVoiceActive())
            voice->renderBlock (output, startSample, numSamples);
        else
            voice->bypassedBlock (numSamples);
    }
}


/*
 This should be called when a block is recieved while the processor is in bypassed mode.
 This will not output any audio, but may alter the contents of the MidiBuffer.
 */
template < typename SampleType >
void SynthBase< SampleType >::bypassedBlock (const int numSamples, MidiBuffer& midiMessages)
{
    processMidi (midiMessages);

    for (auto* voice : voices)
        voice->bypassedBlock (numSamples);
}


/*
 Sets the synth's current playback samplerate.
 This MUST be called before you attempt to render any audio with the synth!
 */
template < typename SampleType >
void SynthBase< SampleType >::setCurrentPlaybackSampleRate (const double newRate)
{
    jassert (newRate > 0);

    sampleRate = newRate;

    for (auto* voice : voices)
        voice->updateSampleRate (newRate);

    samplerateChanged (newRate);
}


/*=========================================================================================================
 __      ______ _____ _____ ______             _      _      ____   _____       _______ _____ ____  _   _
 \ \    / / __ \_   _/ ____|  ____|      /\   | |    | |    / __ \ / ____|   /\|__   __|_   _/ __ \| \ | |
  \ \  / / |  | || || |    | |__        /  \  | |    | |   | |  | | |       /  \  | |    | || |  | |  \| |
   \ \/ /| |  | || || |    |  __|      / /\ \ | |    | |   | |  | | |      / /\ \ | |    | || |  | | . ` |
    \  / | |__| || || |____| |____    / ____ \| |____| |___| |__| | |____ / ____ \| |   _| || |__| | |\  |
     \/   \____/_____\_____|______|  /_/    \_\______|______\____/ \_____/_/    \_\_|  |_____\____/|_| \_|
 
 Functions for managing the synth's collection of voices
 
 ==========================================================================================================*/

/*
 Attempts to find the appropriate voice to start when a note-on event is recieved.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::findFreeVoice (const bool stealIfNoneAvailable)
{
    jassert (! voices.isEmpty());

    /* Look into the future!  If a voice has a note off coming within the next few milliseconds, let's steal that voice... */
    constexpr int futureStealingMaxMs = 10;

    for (auto* voice : voices)
    {
        if (! voice->isVoiceActive()) continue;

        const auto note = voice->getCurrentlyPlayingNote();

        for (auto midiIterator = midiInputStorage.findNextSamplePosition (lastMidiTimeStamp + 1);
             midiIterator != midiInputStorage.findNextSamplePosition (lastMidiTimeStamp + futureStealingMaxMs + 1);
             ++midiIterator)
        {
            const auto metadata = *midiIterator;
            const auto msg      = metadata.getMessage();

            if (msg.isNoteOff() && msg.getNoteNumber() == note) return voice;
        }
    }

    for (auto* voice : voices)
        if (! voice->isVoiceActive()) return voice;

    if (stealIfNoneAvailable) return findVoiceToSteal();

    return nullptr;
}


/*
 If findFreeVoice() is called and every voice is active, this function will attempt to find the optimal voice to "steal" for the new note.
 This voice stealing algorithm protects the highest & lowest notes whose keyboard keys are still being held, and the pedal & descant voices, if they're active.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::findVoiceToSteal()
{
    // These are the voices we want to protect
    Voice* low = nullptr;  // Lowest sounding note, might be sustained, but NOT in release phase
    Voice* top = nullptr;  // Highest sounding note, might be sustained, but NOT in release phase

    // protect these, only use if necessary. These will be nullptrs if pedal / descant is currently off
    Voice* descantVoice = getCurrentDescantVoice();
    Voice* pedalVoice   = getCurrentPedalPitchVoice();

    usableVoices.clearQuick();  // this is a list of voices we can steal, sorted by how long they've been on

    for (auto* voice : voices)
    {
        if (voice == descantVoice || voice == pedalVoice) continue;

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

            if (low == nullptr || note < low->getCurrentlyPlayingNote()) low = voice;

            if (top == nullptr || note > top->getCurrentlyPlayingNote()) top = voice;
        }
    }

    if (top == low)  // Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
        top = nullptr;

    for (auto* voice : usableVoices)
        if (voice != low && voice != top && ! voice->isKeyDown()) return voice;

    for (auto* voice : usableVoices)
        if (voice != low && voice != top) return voice;

    // only protected top & bottom voices are left now - time to use the pedal pitch & descant voices...

    if (descantVoice != nullptr)  // save bass
        return descantVoice;

    if (pedalVoice != nullptr) return pedalVoice;

    // return final top & bottom notes held with keyboard keys

    if (top != nullptr)  // save bass
        return top;

    return low;
}


/*
 Call this function to change the number of voices the synth is managing.
 Note that this function will allocate!
 */
template < typename SampleType >
void SynthBase< SampleType >::changeNumVoices (const int newNumVoices)
{
    const auto currentVoices = voices.size();

    if (currentVoices == newNumVoices) return;

    if (newNumVoices > currentVoices)
        addNumVoices (newNumVoices - currentVoices);
    else
        removeNumVoices (currentVoices - newNumVoices);

    jassert (voices.size() == newNumVoices);
}


/*
 Adds a specified number of voices to the synth.
 This is virtual because your subclass can override this to add instances of your subclass of SynthVoiceBase. If you provide a custom implementation, your function MUST call numVoicesChanged() after it has finished adding voices!
 */
template < typename SampleType >
void SynthBase< SampleType >::addNumVoices (const int voicesToAdd)
{
    if (voicesToAdd == 0) return;

    for (int i = 0; i < voicesToAdd; ++i)
        voices.add (new Voice (this, sampleRate));

    jassert (voices.size() >= voicesToAdd);

    numVoicesChanged();
}


/*
 Removes a specified number of voices from the synth. This function attempts to remove inactive voices first.
 */
template < typename SampleType >
void SynthBase< SampleType >::removeNumVoices (const int voicesToRemove)
{
    if (voicesToRemove == 0) return;

#if JUCE_DEBUG
    const auto shouldBeLeft = voices.size() - voicesToRemove;
#endif

    int voicesRemoved = 0;

    while (voicesRemoved < voicesToRemove && ! voices.isEmpty())
    {
        if (voices.isEmpty()) break;

        Voice* removing = findFreeVoice (true);

        if (removing == nullptr) removing = voices[0];

        if (removing->isVoiceActive())
        {
            panner.panValTurnedOff (removing->getCurrentMidiPan());
            aggregateMidiBuffer.addEvent (MidiMessage::noteOff (removing->getMidiChannel(), removing->getCurrentlyPlayingNote(), 1.0f),
                                          ++lastMidiTimeStamp);
        }

        voices.removeObject (removing, true);

        ++voicesRemoved;
    }

    jassert (voices.isEmpty() || voices.size() == shouldBeLeft);

    numVoicesChanged();
}


/*
 This function should be called any time the synth's number of voices is changed.
 */
template < typename SampleType >
void SynthBase< SampleType >::numVoicesChanged()
{
    const auto newMaxNumVoices = voices.size();

    panner.prepare (newMaxNumVoices, false);
    usableVoices.ensureStorageAllocated (newMaxNumVoices);
    currentNotes.ensureStorageAllocated (newMaxNumVoices);
    desiredNotes.ensureStorageAllocated (newMaxNumVoices);
}


/*
 Returns a pointer to the voice playing a certain note. This will return nullptr if the note is not currently active.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::getVoicePlayingNote (const int midiPitch) const
{
    auto it = std::find_if (voices.begin(),
                            voices.end(),
                            [&midiPitch] (SynthVoiceBase< SampleType >* voice)
                            { return voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch; });

    return it != voices.end() ? *it : nullptr;
}


/*
 Returns a pointer to the voice playing the pitch created by the applyDescant() algorithm. This will return nullptr if the descant is not currently active.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::getCurrentDescantVoice() const
{
    auto it = std::find_if (
        voices.begin(), voices.end(), [] (SynthVoiceBase< SampleType >* voice)
        { return voice->isVoiceActive() && voice->isCurrentDescantVoice(); });

    return it != voices.end() ? *it : nullptr;
}


/*
 Returns a pointer to the voice playing the pitch created by the applyPedalPitch() algorithm. This will return nullptr if the pedal pitch is not currently active.
 */
template < typename SampleType >
SynthVoiceBase< SampleType >* SynthBase< SampleType >::getCurrentPedalPitchVoice() const
{
    auto it = std::find_if (
        voices.begin(), voices.end(), [] (SynthVoiceBase< SampleType >* voice)
        { return voice->isVoiceActive() && voice->isCurrentPedalVoice(); });

    return it != voices.end() ? *it : nullptr;
}


/*
 Returns the number of currently active voices.
 Voices in any state are counted - including playing-but-released, sustaining from sostenuto pedal, etc.
 */
template < typename SampleType >
int SynthBase< SampleType >::getNumActiveVoices() const
{
    return static_cast< int > (
        std::count_if (voices.begin(), voices.end(), [] (SynthVoiceBase< SampleType >* voice)
                       { return voice->isVoiceActive(); }));
}


/*=========================================================================================================
  _____        _____            __  __ ______ _______ ______ _____      __  __          _   _          _____ ______ __  __ ______ _   _ _______
 |  __ \ /\   |  __ \     /\   |  \/  |  ____|__   __|  ____|  __ \    |  \/  |   /\   | \ | |   /\   / ____|  ____|  \/  |  ____| \ | |__   __|
 | |__) /  \  | |__) |   /  \  | \  / | |__     | |  | |__  | |__) |   | \  / |  /  \  |  \| |  /  \ | |  __| |__  | \  / | |__  |  \| |  | |
 |  ___/ /\ \ |  _  /   / /\ \ | |\/| |  __|    | |  |  __| |  _  /    | |\/| | / /\ \ | . ` | / /\ \| | |_ |  __| | |\/| |  __| | . ` |  | |
 | |  / ____ \| | \ \  / ____ \| |  | | |____   | |  | |____| | \ \    | |  | |/ ____ \| |\  |/ ____ \ |__| | |____| |  | | |____| |\  |  | |
 |_| /_/    \_\_|  \_\/_/    \_\_|  |_|______|  |_|  |______|_|  \_\   |_|  |_/_/    \_\_| \_/_/    \_\_____|______|_|  |_|______|_| \_|  |_|
 
 Functions for updating the synth's parameters
 
 ==========================================================================================================*/

/*
 Sets the frequency in Hz corresponding to midi note 69 (A4). This will usually be 440.
 Setting this to values higher or lower than 440 will effective detune the entire synth.
 */
template < typename SampleType >
void SynthBase< SampleType >::setConcertPitchHz (const int newConcertPitchhz)
{
#if bvsb_USE_MTS_ESP
    juce::ignoreUnused (newConcertPitchhz);
#else
    jassert (newConcertPitchhz > 0);

    if (pitchConverter.getCurrentConcertPitchHz() == newConcertPitchhz) return;

    pitchConverter.setConcertPitchHz (newConcertPitchhz);

    for (auto* voice : voices)
        if (voice->isVoiceActive()) voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
#endif
}


/*
 Sets the synth's sensitivity to midi velocity.
 At 100, each voice's volume is entirely dependant on the velocity of its note-on message and subsequent aftertouch messages.
 At 0, velocity and aftertouch have no effect on the voices' volume.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateMidiVelocitySensitivity (int newSensitivity)
{
    jassert (newSensitivity >= 0 && newSensitivity <= 100);

    const auto newSens = newSensitivity / 100.0f;

    if (velocityConverter.getCurrentSensitivity() == newSens) return;

    velocityConverter.setFloatSensitivity (newSens);

    for (auto* voice : voices)
        if (voice->isVoiceActive()) voice->setVelocityMultiplier (getWeightedVelocity (voice->getLastRecievedVelocity()));
}


/*
 Sets the range of the pitch wheel up and down, in semitones.
 */
template < typename SampleType >
void SynthBase< SampleType >::updatePitchbendSettings (const int rangeUp, const int rangeDown)
{
    if ((bendTracker.getCurrentRangeUp() == rangeUp) && (bendTracker.getCurrentRangeDown() == rangeDown)) return;

    bendTracker.setRange (rangeUp, rangeDown);

    if (lastPitchWheelValue == 64) return;

    for (auto* voice : voices)
        if (voice->isVoiceActive()) voice->setCurrentOutputFreq (getOutputFrequency (voice->getCurrentlyPlayingNote()));
}


/*
  ___ _____ ___ ___ ___ ___    __      _____ ___ _____ _  _
 / __|_   _| __| _ \ __/ _ \   \ \    / /_ _|   \_   _| || |
 \__ \ | | | _||   / _| (_) |   \ \/\/ / | || |) || | | __ |
 |___/ |_| |___|_|_\___\___/     \_/\_/ |___|___/ |_| |_||_|
 
 Parameters to control the stereo image of the synth's output
 */

/*
 Updates the stereo width of the synthesizer as a whole.
 At 100, the active voices will be evenly spread across the entire stereo field. At 0, the synth will produce mono output.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateStereoWidth (int newWidth)
{
    jassert (newWidth >= 0 && newWidth <= 100);

    if (panner.getCurrentStereoWidth() == newWidth) return;

    panner.updateStereoWidth (newWidth);

    for (auto* voice : voices)
    {
        if (! voice->isVoiceActive()) continue;

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


/*
 Sets the lowest midi pitch that will be panned. Pitches below this threshold will always be placed dead center.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateLowestPannedNote (int newPitchThresh)
{
    jassert (newPitchThresh >= 0 && newPitchThresh <= 127);

    const auto prevLowestnote = lowestPannedNote;

    if (prevLowestnote == newPitchThresh) return;

    lowestPannedNote = newPitchThresh;

    for (auto* voice : voices)
    {
        if (! voice->isVoiceActive()) continue;

        const auto note       = voice->getCurrentlyPlayingNote();
        const auto currentPan = voice->getCurrentMidiPan();

        if (note < newPitchThresh)
        {
            if (currentPan != 64)
            {
                panner.panValTurnedOff (currentPan);
                voice->setPan (64);
            }
        }
        else if (
            note
            < prevLowestnote)  // because we haven't updated the lowestPannedNote member variable yet, voices with pitches higher than newPitchThresh but lower than lowestPannedNote are the voices that now qualify for panning
        {
            if (currentPan == 64) voice->setPan (panner.getNextPanVal());
        }
    }
}


/*
    _   ___  ___ ___ _ ___
   /_\ |   \/ __| _ ( ) __|
  / _ \| |) \__ \   //\__ \
 /_/ \_\___/|___/_|_\ |___/
 
 Parameters to control the synth's various ADSRs
 */

/*
 Updates the settings for the main midi-triggered ADSR.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateADSRsettings (const float attack, const float decay, const float sustain, const float release)
{
    // attack/decay/release time in SECONDS; sustain ratio 0.0 - 1.0

    adsrParams.attack  = attack;
    adsrParams.decay   = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    for (auto* voice : voices)
        voice->setAdsrParameters (adsrParams);
}


/*
 Updates the synth's quick release time, in milliseconds.
 This is used to prevent clicks when stopping notes with no tail off, etc.
 */
template < typename SampleType >
void SynthBase< SampleType >::updateQuickReleaseMs (const int newMs)
{
    const auto desiredR = newMs * 0.001f;

    quickReleaseParams.release = desiredR;

    for (auto* voice : voices)
        voice->setQuickReleaseParameters (quickReleaseParams);
}


/*
  ___ ___ ___   _   _       ___ ___ _____ ___ _  _
 | _ \ __|   \ /_\ | |     | _ \_ _|_   _/ __| || |
 |  _/ _|| |) / _ \| |__   |  _/| |  | || (__| __ |
 |_| |___|___/_/ \_\____|  |_| |___| |_| \___|_||_|
 
 Parameters to control the automated pedal pitch feature
 */

/*
 Toggles pedal pitch on or off.
 */
template < typename SampleType >
void SynthBase< SampleType >::setPedalPitch (const bool isOn)
{
    if (pedal.isOn == isOn) return;

    pedal.isOn = isOn;

    if (isOn)
        applyPedalPitch();
    else
    {
        if (pedal.lastPitch > -1) noteOff (pedal.lastPitch, 1.0f, false, false);

        pedal.lastPitch = -1;
    }
}


/*
 Sets the interval, in semitones, at which the pedal pitch voice will double the lowest played keyboard note.
 EG. 12 will make the pedal pitch feature double the lowest played note an octave lower, etc.
 */
template < typename SampleType >
void SynthBase< SampleType >::setPedalPitchInterval (const int newInterval)
{
    if (pedal.interval == newInterval) return;

    pedal.interval = newInterval;

    if (pedal.isOn) applyPedalPitch();
}


/*
 Sets the pitch threshold for the pedal pitch feature.
 The pedal pitch voice will only be created if the lowest played keyboard key is below the pitch set here.
 */
template < typename SampleType >
void SynthBase< SampleType >::setPedalPitchUpperThresh (int newThresh)
{
    jassert (newThresh >= 0 && newThresh <= 127);

    if (pedal.upperThresh == newThresh) return;

    pedal.upperThresh = newThresh;

    if (pedal.isOn) applyPedalPitch();
}


/*
  ___  ___ ___  ___   _   _  _ _____
 |   \| __/ __|/ __| /_\ | \| |_   _|
 | |) | _|\__ \ (__ / _ \| .` | | |
 |___/|___|___/\___/_/ \_\_|\_| |_|
 
 Parameters to control the automated descant feature
 */

/*
 Toggles descant on or off.
 */
template < typename SampleType >
void SynthBase< SampleType >::setDescant (const bool isOn)
{
    if (descant.isOn == isOn) return;

    descant.isOn = isOn;

    if (isOn)
        applyDescant();
    else
    {
        if (descant.lastPitch > -1) noteOff (descant.lastPitch, 1.0f, false, false);

        descant.lastPitch = -1;
    }
}


/*
 Sets the interval, in semitones, at which the descant voice will double the highest played keyboard note.
 EG. 12 will make the descant feature double the highest played note an octave higher, etc.
 */
template < typename SampleType >
void SynthBase< SampleType >::setDescantInterval (const int newInterval)
{
    if (descant.interval == newInterval) return;

    descant.interval = newInterval;

    if (descant.isOn) applyDescant();
}


/*
 Sets the pitch threshold for the descant feature.
 The descant voice will only be created if the highest played keyboard key is above the pitch set here.
 */
template < typename SampleType >
void SynthBase< SampleType >::setDescantLowerThresh (int newThresh)
{
    jassert (newThresh >= 0 && newThresh <= 127);

    if (descant.lowerThresh == newThresh) return;

    descant.lowerThresh = newThresh;

    if (descant.isOn) applyDescant();
}


/*=========================================================================================================
  __  __ _____  _____  _____      ______ _    _ _   _  _____ _______ _____ ____  _   _  _____
 |  \/  |_   _|/ ____|/ ____|    |  ____| |  | | \ | |/ ____|__   __|_   _/ __ \| \ | |/ ____|
 | \  / | | | | (___ | |         | |__  | |  | |  \| | |       | |    | || |  | |  \| | (___
 | |\/| | | |  \___ \| |         |  __| | |  | | . ` | |       | |    | || |  | | . ` |\___ \
 | |  | |_| |_ ____) | |____ _   | |    | |__| | |\  | |____   | |   _| || |__| | |\  |____) |
 |_|  |_|_____|_____/ \_____(_)  |_|     \____/|_| \_|\_____|  |_|  |_____\____/|_| \_|_____/
 
 Functions that don't really belong anywhere else
 
 ==========================================================================================================*/

/*
 Returns a float velocity, weighted according to the current midi velocity sensitivity setting.
 The voices call this for setting both their velocity gain and their aftertouch gain -- aftertouch is also weighted according to midi velocity sensitivity.
 */
template < typename SampleType >
float SynthBase< SampleType >::getWeightedVelocity (const float inputFloatVelocity) const
{
    jassert (inputFloatVelocity >= 0.0f && inputFloatVelocity <= 1.0f);
    return velocityConverter.floatVelocity (inputFloatVelocity);
}


/*
 Returns a frequency in Hz for a voice to play, based on its recieved midi note. The output frequency takes into account the current pitchwheel position and the current concert pitch Hz setting.
 */
template < typename SampleType >
float SynthBase< SampleType >::getOutputFrequency (const int midipitch, const int midiChannel) const
{
#if bvsb_USE_MTS_ESP
    return bav::math::midiToFreq (
        bendTracker.newNoteRecieved (bav::math::freqToMidi (MTS_NoteToFrequency (mtsClient, char (midipitch), char (midiChannel)))));
#else
    juce::ignoreUnused (midiChannel);
    return pitchConverter.mtof (bendTracker.newNoteRecieved (midipitch));
#endif
}


/*
 Returns true if the synth is currently ignoring the midi pitch for the passed midi channel (pass -1 for ÏDK the channel" or äll midi channels")
 */
template < typename SampleType >
bool SynthBase< SampleType >::shouldFilterNote (int midiNote, int midiChannel) const
{
#if bvsb_USE_MTS_ESP
    return MTS_ShouldFilterNote (mtsClient, char (midiNote), char (midiChannel));
#else
    juce::ignoreUnused (midiNote, midiChannel);
    return false;
#endif
}


/*
 Resets the voices' ramped gain values, and prepares them for a new blocksize.
 This should be called with/by prepare().
 */
template < typename SampleType >
void SynthBase< SampleType >::resetRampedValues (int blocksize)
{
    for (auto* voice : voices)
        voice->resetRampedValues (blocksize);
}


template < typename SampleType >
bool SynthBase< SampleType >::isConnectedToMtsEsp() const noexcept
{
#if bvsb_USE_MTS_ESP
    return MTS_HasMaster (mtsClient);
#else
    return false;
#endif
}

template < typename SampleType >
juce::String SynthBase< SampleType >::getScaleName() const
{
#if bvsb_USE_MTS_ESP
    return {MTS_GetScaleName (mtsClient)};
#else
    return {};
#endif
}


template class SynthBase< float >;
template class SynthBase< double >;


}  // namespace bav::dsp
