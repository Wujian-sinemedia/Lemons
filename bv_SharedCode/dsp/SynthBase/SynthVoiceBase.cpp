

namespace bav::dsp
{
    
///multiplicative smoothing cannot ever actually reach 0
#define bv_MIN_SMOOTHED_GAIN 0.0000001
#define _SMOOTHING_ZERO_CHECK(inputGain) std::max(SampleType(bv_MIN_SMOOTHED_GAIN), SampleType (inputGain))
    
    
    template<typename SampleType>
    SynthVoiceBase<SampleType>::SynthVoiceBase(SynthBase<SampleType>* base, double initSamplerate):
            parent(base),
            keyIsDown(false), playingButReleased(false), sustainingFromSostenutoPedal(false), isQuickFading(false), noteTurnedOff(false),
            currentlyPlayingNote(-1), currentAftertouch(0), currentOutputFreq(-1.0f),  lastRecievedVelocity(0.0f), noteOnTime(0),
            isPedalPitchVoice(false), isDescantVoice(false),
            renderingBuffer(0, 0), stereoBuffer(0, 0)
    {
        adsr        .setSampleRate (initSamplerate);
        quickRelease.setSampleRate (initSamplerate);
        quickAttack .setSampleRate (initSamplerate);
        adsr        .setParameters (parent->getCurrentAdsrParams());
        quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
        quickAttack .setParameters (parent->getCurrentQuickAttackParams());
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::prepare (const int blocksize)
    {
        renderingBuffer.setSize (1, blocksize, true, true, true);
        stereoBuffer.setSize (2, blocksize, true, true, true);
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::renderBlock (AudioBuffer& output)
    {
        const bool voiceIsOnRightNow = isQuickFading ? quickRelease.isActive()
        : ( parent->isADSRon() ? adsr.isActive() : ! noteTurnedOff );
        if (! voiceIsOnRightNow)
        {
            clearCurrentNote();
            return;
        }
        
        jassert (output.getNumSamples() > 0 && output.getNumChannels() > 0);
        
        jassert (currentOutputFreq > 0);
        jassert (parent->getSamplerate() > 0);
        jassert (renderingBuffer.getNumChannels() > 0);
        
        const int numSamples = output.getNumSamples();
        
        //  alias buffer containing exactly the # of samples we want this frame
        AudioBuffer render (renderingBuffer.getArrayOfWritePointers(), 1, 0, numSamples);
        
        //  generate some mono audio output at the frequency currentOutputFreq.....
        renderPlease (render, currentOutputFreq, parent->getSamplerate());
        
        //  smoothed gain modulations
        midiVelocityGain.applyGain       (render, numSamples);
        softPedalGain.applyGain          (render, numSamples);
        playingButReleasedGain.applyGain (render, numSamples);
        aftertouchGain.applyGain         (render, numSamples);
        
        //  ADSR
        if (parent->isADSRon())
            adsr.applyEnvelopeToBuffer (render, 0, numSamples); // midi-triggered adsr envelope
        else
            quickAttack.applyEnvelopeToBuffer (render, 0, numSamples); // to prevent pops at start of notes if adsr is off
        
        if (isQuickFading)  // quick fade out for stopNote w/ no tail off, to prevent clicks from jumping to 0
            quickRelease.applyEnvelopeToBuffer (render, 0, numSamples);
        
        if (output.getNumChannels() == 1)
            output.addFrom (0, 0, render, 0, 0, numSamples);
        else
        {
            //  write to stereoBuffer and apply panning
            stereoBuffer.copyFrom (0, 0, render, 0, 0, numSamples);
            stereoBuffer.copyFrom (1, 0, render, 0, 0, numSamples);
            outputLeftGain.applyGain  (stereoBuffer.getWritePointer(0), numSamples);
            outputRightGain.applyGain (stereoBuffer.getWritePointer(1), numSamples);
            
            //  add (!) to output
            for (int chan = 0; chan < 2; ++chan)
                output.addFrom (chan, 0, stereoBuffer, chan, 0, numSamples);
        }
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::bypassedBlock (const int numSamples)
    {
        midiVelocityGain.skip (numSamples);
        softPedalGain.skip (numSamples);
        playingButReleasedGain.skip (numSamples);
        aftertouchGain.skip (numSamples);
        outputLeftGain.skip (numSamples);
        outputRightGain.skip (numSamples);
        bypassedBlockRecieved (numSamples);
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::resetRampedValues (int blocksize)
    {
        midiVelocityGain.reset (blocksize);
        softPedalGain.reset (blocksize);
        playingButReleasedGain.reset (blocksize);
        aftertouchGain.reset (blocksize);
        outputLeftGain.reset (blocksize);
        outputRightGain.reset (blocksize);
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::clearCurrentNote()
    {
        lastRecievedVelocity = 0.0f;
        currentAftertouch = 0;
        currentlyPlayingNote = -1;
        noteOnTime = 0;
        isQuickFading = false;
        noteTurnedOff = true;
        keyIsDown = false;
        playingButReleased = false;
        sustainingFromSostenutoPedal = false;
        isPedalPitchVoice = false;
        isDescantVoice = false;
        
        if (quickRelease.isActive())
            quickRelease.reset();
        
        quickRelease.noteOn();
        
        if (adsr.isActive())
            adsr.reset();
        
        if (quickAttack.isActive())
            quickAttack.reset();
        
        setPan (64);
        
        resetRampedValues (parent->getLastBlocksize());
        
        noteCleared();
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::startNote (const int midiPitch,  const float velocity,
                                                const uint32 noteOnTimestamp,
                                                const bool keyboardKeyIsDown = true,
                                                const bool isPedal, const bool isDescant)
    {
        noteOnTime = noteOnTimestamp;
        currentlyPlayingNote = midiPitch;
        lastRecievedVelocity = velocity;
        currentOutputFreq = parent->getOutputFrequency (midiPitch);
        isQuickFading = false;
        noteTurnedOff = false;
        isPedalPitchVoice = isPedal;
        isDescantVoice = isDescant;
        
        adsr.noteOn();
        quickAttack.noteOn();
        
        if (! quickRelease.isActive())
            quickRelease.noteOn();
        
        setKeyDown (keyboardKeyIsDown);
        
        midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getWeightedVelocity (velocity)));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::stopNote (const float velocity, const bool allowTailOff)
    {
        midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getWeightedVelocity (lastRecievedVelocity - velocity)));
        
        if (allowTailOff)
        {
            adsr.noteOff();
            isQuickFading = false;
        }
        else
        {
            if (! quickRelease.isActive())
                quickRelease.noteOn();
            
            isQuickFading = true;
            
            quickRelease.noteOff();
        }
        
        noteTurnedOff = true;
        keyIsDown = false;
        playingButReleased = false;
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::updateSampleRate (const double newSamplerate)
    {
        adsr        .setSampleRate (newSamplerate);
        quickRelease.setSampleRate (newSamplerate);
        quickAttack .setSampleRate (newSamplerate);
        
        adsr        .setParameters (parent->getCurrentAdsrParams());
        quickRelease.setParameters (parent->getCurrentQuickReleaseParams());
        quickAttack .setParameters (parent->getCurrentQuickAttackParams());
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::setKeyDown (bool isNowDown)
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
            playingButReleasedGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getPlayingButReleasedMultiplier()));
        else
            playingButReleasedGain.setTargetValue (SampleType(1.0));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::setPan (int newPan)
    {
        newPan = juce::jlimit(0, 127, newPan);
        
        if (panner.getLastMidiPan() == newPan)
            return;
        
        panner.setMidiPan (newPan);
        
        outputLeftGain.setTargetValue (_SMOOTHING_ZERO_CHECK (panner.getLeftGain()));
        outputRightGain.setTargetValue (_SMOOTHING_ZERO_CHECK (panner.getRightGain()));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::setVelocityMultiplier (const float newMultiplier)
    {
        midiVelocityGain.setTargetValue (_SMOOTHING_ZERO_CHECK(newMultiplier));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::softPedalChanged (bool isDown)
    {
        if (isDown)
            softPedalGain.setTargetValue (_SMOOTHING_ZERO_CHECK (parent->getSoftPedalMultiplier()));
        else
            softPedalGain.setTargetValue (SampleType(1.0));
    }
    
    
    template<typename SampleType>
    void SynthVoiceBase<SampleType>::aftertouchChanged (const int newAftertouchValue)
    {
        currentAftertouch = newAftertouchValue;
        
        constexpr auto inv127 = SampleType (1.0 / 127.0);
        
        if (parent->isAftertouchGainOn())
            aftertouchGain.setTargetValue (_SMOOTHING_ZERO_CHECK (newAftertouchValue * inv127));
        else
            aftertouchGain.setTargetValue (SampleType(1.0));
    }
    
    
#undef bv_MIN_SMOOTHED_GAIN
#undef _SMOOTHING_ZERO_CHECK
    

///explicit class instantiations
template class SynthVoiceBase<float>;
template class SynthVoiceBase<double>;
    
    
}  // namespace
