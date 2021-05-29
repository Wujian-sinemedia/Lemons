

namespace bav::dsp
{
/*
        Constructor.
    */
template < typename SampleType >
SynthVoiceBase< SampleType >::SynthVoiceBase (SynthBase< SampleType >* base, double initSamplerate)
    : parent (base), keyIsDown (false), playingButReleased (false), sustainingFromSostenutoPedal (false), isQuickFading (false), currentlyPlayingNote (-1), currentAftertouch (0), currentOutputFreq (-1.0f), lastRecievedVelocity (0.0f), noteOnTime (0), isPedalPitchVoice (false), isDescantVoice (false), isDoubledByAutomatedVoice (false), renderingBuffer (0, 0), stereoBuffer (0, 0)
{
    adsr.setSampleRate (initSamplerate);
    quickRelease.setSampleRate (initSamplerate);
    adsr.setParameters (parent->getCurrentAdsrParams());
    quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
}


/*
        Prepares the voice for a new expected maximum blocksize.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::prepare (const int blocksize)
{
    renderingBuffer.setSize (1, blocksize, true, true, true);
    stereoBuffer.setSize (2, blocksize, true, true, true);
    prepared (blocksize);
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
        Renders the next block of output.
        Note that startSample may not correspond to sample 0 of the output buffer!
        If you override this function, you should only modify the samples between startSample and (startSample + numSamples - 1) within the output buffer, and you should ADD!!! to the output buffer, as opposed to writing over its content.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::renderBlock (AudioBuffer& output, const int startSample, const int numSamples)
{
    const bool voiceIsOnRightNow = isQuickFading ? quickRelease.isActive() : adsr.isActive();

    if (! voiceIsOnRightNow)
    {
        clearCurrentNote();
        return;
    }

    if (numSamples == 0) return;

    //  it's possible that the MTS-ESP master tuning table has changed since the last time this function was called...
    if (parent->shouldFilterNote (currentlyPlayingNote, midiChannel))
        stopNote (1.0f, false);
    else
        currentOutputFreq = parent->getOutputFrequency (currentlyPlayingNote, midiChannel);

    jassert (output.getNumSamples() >= startSample + numSamples);
    jassert (parent->getSamplerate() > 0);
    jassert (renderingBuffer.getNumChannels() > 0);
    jassert (currentOutputFreq > 0);

    //  alias buffer containing exactly the # of samples we want this frame
    AudioBuffer render (renderingBuffer.getArrayOfWritePointers(), 1, 0, numSamples);

    vecops::fill (render.getWritePointer (0), SampleType (0), numSamples);

    //  generate some mono audio output at the frequency currentOutputFreq.....
    //  the subclass should write its output to "render" (which is an alias for renderingBuffer)
    renderPlease (render, currentOutputFreq, parent->getSamplerate(), startSample);

    //  smoothed gain modulations
    midiVelocityGain.applyGain (render, numSamples);
    aftertouchGain.applyGain (render, numSamples);
    softPedalGain.applyGain (render, numSamples);
    playingButReleasedGain.applyGain (render, numSamples);

    adsr.applyEnvelopeToBuffer (render, 0, numSamples);  // midi-triggered adsr envelope

    // quick fade out for stopNote w/ no tail off, to prevent clicks from output suddenly jumping to 0
    if (isQuickFading)
        quickRelease.applyEnvelopeToBuffer (render, 0, numSamples);
    else
        for (int i = 0; i < numSamples; ++i)
            quickRelease.getNextSample();

    if (output.getNumChannels() == 1)
    {
        //  add (!) to output
        vecops::addV (output.getWritePointer (0, startSample), render.getReadPointer (0), numSamples);
    }
    else
    {
        //  copy to stereoBuffer
        for (int chan = 0; chan < 2; ++chan)
            vecops::copy (render.getReadPointer (0), stereoBuffer.getWritePointer (chan), numSamples);

        //  apply panning
        outputLeftGain.applyGain (stereoBuffer.getWritePointer (0), numSamples);
        outputRightGain.applyGain (stereoBuffer.getWritePointer (1), numSamples);

        //  add (!) to output
        for (int chan = 0; chan < 2; ++chan)
            vecops::addV (output.getWritePointer (chan, startSample), stereoBuffer.getReadPointer (chan), numSamples);
    }

    const bool voiceIsStillOn = isQuickFading ? quickRelease.isActive() : adsr.isActive();

    if (! voiceIsStillOn) clearCurrentNote();
}


/*
        Should be called when a block is recieved but the voice is not active, or is in a bypassed state.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::bypassedBlock (const int numSamples)
{
    midiVelocityGain.skip (numSamples);
    softPedalGain.skip (numSamples);
    playingButReleasedGain.skip (numSamples);
    aftertouchGain.skip (numSamples);
    outputLeftGain.skip (numSamples);
    outputRightGain.skip (numSamples);

    bypassedBlockRecieved (currentOutputFreq, parent->getSamplerate(), numSamples);
}


/*
        Called when the samplerate of the parent changes.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::updateSampleRate (const double newSamplerate)
{
    adsr.setSampleRate (newSamplerate);
    quickRelease.setSampleRate (newSamplerate);

    adsr.setParameters (parent->getCurrentAdsrParams());
    quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
}


/*=========================================================================================================
      __  __ _____ _____ _____
     |  \/  |_   _|  __ \_   _|
     | \  / | | | | |  | || |
     | |\/| | | | | |  | || |
     | |  | |_| |_| |__| || |_
     |_|  |_|_____|_____/_____|
     
     Functions for midi events and features
     
     ==========================================================================================================*/

/*
        Starts the desired note.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::startNote (const int    midiPitch,
                                              const float  velocity,
                                              const uint32 noteOnTimestamp,
                                              const bool   keyboardKeyIsDown,
                                              const bool   isPedal,
                                              const bool   isDescant,
                                              const int    midichannel)
{
    noteOnTime           = noteOnTimestamp;
    currentlyPlayingNote = midiPitch;
    lastRecievedVelocity = velocity;
    currentOutputFreq    = parent->getOutputFrequency (midiPitch, midichannel);
    isQuickFading        = false;
    isPedalPitchVoice    = isPedal;
    isDescantVoice       = isDescant;

    midiChannel = midichannel;

    adsr.noteOn();
    quickRelease.noteOn();

    setKeyDown (keyboardKeyIsDown);

    midiVelocityGain.setTargetValue (smoothingZeroCheck (parent->getWeightedVelocity (velocity)));

    aftertouchGain.setTargetValue (SampleType (1.0));
    
    if (isPedal || isDescant)
        isDoubledByAutomatedVoice = false;
}


/*
        Stops the voice's currently active note.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::stopNote (const float velocity, const bool allowTailOff)
{
    const auto newGain = juce::jlimit (0.0f, 1.0f, lastRecievedVelocity - velocity);
    midiVelocityGain.setTargetValue (smoothingZeroCheck (parent->getWeightedVelocity (newGain)));

    adsr.noteOff();
    quickRelease.noteOff();

    isQuickFading = ! allowTailOff;

    keyIsDown          = false;
    playingButReleased = false;
}


/*
        Returns the voice to its initial/neutral state.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::clearCurrentNote()
{
    lastRecievedVelocity         = 0.0f;
    currentAftertouch            = 0;
    currentlyPlayingNote         = -1;  // NB: this function does NOT change currentOutputFreq and that is intentional!!
    noteOnTime                   = 0;
    isQuickFading                = false;
    keyIsDown                    = false;
    playingButReleased           = false;
    sustainingFromSostenutoPedal = false;
    isPedalPitchVoice            = false;
    isDescantVoice               = false;
    isDoubledByAutomatedVoice    = false;

    if (quickRelease.isActive()) quickRelease.reset();

    quickRelease.noteOn();

    if (adsr.isActive()) adsr.reset();

    setPan (64);

    resetRampedValues (parent->getLastBlocksize());

    noteCleared();
}


/*
        Sets the voice's midi velocity multiplier.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::setVelocityMultiplier (const float newMultiplier)
{
    midiVelocityGain.setTargetValue (smoothingZeroCheck (newMultiplier));
}


/*
        Called by the parent synth object when the soft pedal is depressed or raised.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::softPedalChanged (bool isDown)
{
    if (isDown)
        softPedalGain.setTargetValue (smoothingZeroCheck (parent->getSoftPedalMultiplier()));
    else
        softPedalGain.setTargetValue (SampleType (1.0));
}


/*
        Called by the parent object when the voice's assigned aftertouch value changes.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::aftertouchChanged (const int newAftertouchValue)
{
    currentAftertouch = newAftertouchValue;

    constexpr auto inv127 = 1.0f / 127.0f;

    if (parent->isAftertouchGainOn())
    {
        const auto newWeightedGain = juce::jlimit (0.0f, 1.0f, lastRecievedVelocity + newAftertouchValue * inv127);

        aftertouchGain.setTargetValue (smoothingZeroCheck (parent->getWeightedVelocity (newWeightedGain)));
    }
    else
        aftertouchGain.setTargetValue (SampleType (1.0));
}


/*=========================================================================================================
      __  __ _____  _____  _____      ______ _    _ _   _  _____ _______ _____ ____  _   _  _____
     |  \/  |_   _|/ ____|/ ____|    |  ____| |  | | \ | |/ ____|__   __|_   _/ __ \| \ | |/ ____|
     | \  / | | | | (___ | |         | |__  | |  | |  \| | |       | |    | || |  | |  \| | (___
     | |\/| | | |  \___ \| |         |  __| | |  | | . ` | |       | |    | || |  | | . ` |\___ \
     | |  | |_| |_ ____) | |____ _   | |    | |__| | |\  | |____   | |   _| || |__| | |\  |____) |
     |_|  |_|_____|_____/ \_____(_)  |_|     \____/|_| \_|\_____|  |_|  |_____\____/|_| \_|_____/
     
     Miscellaneous functions
     
     ==========================================================================================================*/

/*
        Resets the voice's ramped gain values.
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::resetRampedValues (int blocksize)
{
    midiVelocityGain.reset (blocksize);
    softPedalGain.reset (blocksize);
    playingButReleasedGain.reset (blocksize);
    aftertouchGain.reset (blocksize);
    outputLeftGain.reset (blocksize);
    outputRightGain.reset (blocksize);
}


/*
        Resets the voice's internal tracking of the state of the midi keyboard key for the note it's currently playing
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::setKeyDown (bool isNowDown)
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
        playingButReleasedGain.setTargetValue (smoothingZeroCheck (parent->getPlayingButReleasedMultiplier()));
    else
        playingButReleasedGain.setTargetValue (SampleType (1.0));
}


/*
        Sets the voice's pan
    */
template < typename SampleType >
void SynthVoiceBase< SampleType >::setPan (int newPan)
{
    newPan = juce::jlimit (0, 127, newPan);

    if (panner.getLastMidiPan() == newPan) return;

    panner.setMidiPan (newPan);

    outputLeftGain.setTargetValue (smoothingZeroCheck (panner.getLeftGain()));
    outputRightGain.setTargetValue (smoothingZeroCheck (panner.getRightGain()));
}


template class SynthVoiceBase< float >;
template class SynthVoiceBase< double >;

}  // namespace bav::dsp
