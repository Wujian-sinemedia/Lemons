

namespace lemons::dsp
{
template <typename SampleType>
SynthVoiceBase<SampleType>::SynthVoiceBase (SynthBase<SampleType>* base, double initSamplerate)
	: parent (base)
{
	adsr.setSampleRate (initSamplerate);
	quickRelease.setSampleRate (initSamplerate);
	adsr.setParameters (parent->adsrParams);
	quickRelease.setParameters (parent->quickReleaseParams);
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::prepare (double samplerate, int blocksize)
{
	scratchBuffer.setSize (1, blocksize, true, true, true);
	renderingBuffer.setSize (1, blocksize, true, true, true);
	stereoBuffer.setSize (2, blocksize, true, true, true);

	midiVelocityGain.prepare (parent->sampleRate, blocksize);
	aftertouchGain.prepare (parent->sampleRate, blocksize);

	playingButReleasedMod.prepare (blocksize, samplerate);
	softPedalMod.prepare (blocksize, samplerate);

	adsr.setSampleRate (samplerate);
	quickRelease.setSampleRate (samplerate);

	adsr.setParameters (parent->adsrParams);
	quickRelease.setParameters (parent->quickReleaseParams);

	// setPitchGlideTime (pitchGlideTimeSecs);

	prepared (samplerate, blocksize);
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

template <typename SampleType>
void SynthVoiceBase<SampleType>::renderBlock (AudioBuffer<SampleType>& output)
{
	if (! isVoiceOnRightNow())
	{
		clearCurrentNote();
		return;
	}

	//  it's possible that the MTS-ESP master tuning table has changed since the last time this function was called...
	if (parent->pitch.tuning.shouldFilterNote (currentlyPlayingNote, midiChannel))
		stopNote (1.0f, false);
	else
		setTargetOutputFrequency (parent->pitch.getFrequencyForMidi (currentlyPlayingNote, midiChannel));

	const auto numSamples = output.getNumSamples();
	if (numSamples == 0) return;

	jassert (parent->sampleRate > 0);
	jassert (renderingBuffer.getNumChannels() > 0);

	scratchBuffer.clear();
	renderingBuffer.clear();

	// puts generated audio samples into renderingBuffer
	renderInternal (numSamples);

	// alias buffer containing the # of samples for this frame
	auto render = buffers::getAliasBuffer (renderingBuffer, 0, numSamples, 1);

	//  smoothed gain modulations
	midiVelocityGain.process (render);
	aftertouchGain.process (render);

	playingButReleasedMod.process (render);
	softPedalMod.process (render);

	adsr.applyEnvelopeToBuffer (render, 0, numSamples);	 // midi-triggered adsr envelope

	// quick fade out for stopNote w/ no tail off, to prevent clicks from output suddenly jumping to 0
	if (isQuickFading)
		quickRelease.applyEnvelopeToBuffer (render, 0, numSamples);
	else
		for (int i = 0; i < numSamples; ++i)
			quickRelease.getNextSample();

	using FVO = juce::FloatVectorOperations;

	if (output.getNumChannels() == 1)
	{
		//  add (!) to output
		FVO::add (output.getWritePointer (0), render.getReadPointer (0), numSamples);
	}
	else
	{
		panner.process (render, stereoBuffer);

		//  add (!) to output
		for (int chan = 0; chan < 2; ++chan)
			FVO::add (output.getWritePointer (chan), stereoBuffer.getReadPointer (chan), numSamples);
	}

	if (! isVoiceOnRightNow()) clearCurrentNote();
}

template <typename SampleType>
void SynthVoiceBase<SampleType>::renderInternal (int totalNumSamples)
{
	int samplesProcessed = 0;

	while (samplesProcessed < totalNumSamples)
	{
		if (pitchGlide && outputFrequency.isSmoothing())
		{
			// process a single sample
			auto alias = buffers::getAliasBuffer (scratchBuffer, 0, 1, 1);

			renderPlease (alias, static_cast<float> (outputFrequency.getNextValue()), parent->sampleRate);

			renderingBuffer.setSample (0,
									   samplesProcessed++,
									   alias.getSample (0, 0));

			continue;
		}

		// process the rest of the frame
		const auto samplesLeft = totalNumSamples - samplesProcessed;

		auto alias = buffers::getAliasBuffer (scratchBuffer, 0, samplesLeft, 1);

		renderPlease (alias, static_cast<float> (outputFrequency.getNextValue()), parent->sampleRate);

		// copy to output (rendering buffer)
		juce::FloatVectorOperations::copy (renderingBuffer.getWritePointer (0, samplesProcessed),
										   scratchBuffer.getReadPointer (0),
										   samplesLeft);

		return;
	}
}

template <typename SampleType>
bool SynthVoiceBase<SampleType>::isVoiceOnRightNow() const
{
	if (isQuickFading)
		return quickRelease.isActive();

	return adsr.isActive();
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::bypassedBlock (int numSamples)
{
	midiVelocityGain.bypassedBlock (numSamples);
	aftertouchGain.bypassedBlock (numSamples);

	playingButReleasedMod.skipSamples (numSamples);
	softPedalMod.skipSamples (numSamples);

	bypassedBlockReceived (static_cast<float> (outputFrequency.getTargetValue()), parent->sampleRate, numSamples);
	outputFrequency.skip (numSamples);
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

template <typename SampleType>
void SynthVoiceBase<SampleType>::startNote (const int	 midiPitch,
											const float	 velocity,
											const uint32 noteOnTimestamp,
											const bool	 keyboardKeyIsDown,
											const bool	 isPedal,
											const bool	 isDescant,
											const int	 midichannel)
{
	setTargetOutputFrequency (parent->pitch.getFrequencyForMidi (midiPitch, midichannel));

	noteOnTime			 = noteOnTimestamp;
	currentlyPlayingNote = midiPitch;
	lastReceivedVelocity = velocity;
	isQuickFading		 = false;
	isPedalPitchVoice	 = isPedal;
	isDescantVoice		 = isDescant;

	midiChannel = midichannel;

	adsr.noteOn();
	quickRelease.noteOn();

	setKeyDown (keyboardKeyIsDown);

	midiVelocityGain.setGain (parent->velocityConverter.getGainForVelocity (velocity));
	aftertouchGain.setGain (1.0f);

	if (isPedal || isDescant)
		isDoubledByAutomatedVoice = false;
}

template <typename SampleType>
void SynthVoiceBase<SampleType>::setTargetOutputFrequency (float newFreq)
{
	outputFrequency.set (newFreq, ! isVoiceActive());
}

template <typename SampleType>
void SynthVoiceBase<SampleType>::setPitchGlideTime (double glideTimeSeconds)
{
	pitchGlideTimeSecs = glideTimeSeconds;
	outputFrequency.reset (parent->sampleRate, glideTimeSeconds);
}

template <typename SampleType>
void SynthVoiceBase<SampleType>::togglePitchGlide (bool shouldGlide)
{
	pitchGlide = shouldGlide;
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::stopNote (const float velocity, const bool allowTailOff)
{
	const auto newGain = juce::jlimit (0.0f, 1.0f, lastReceivedVelocity - velocity);
	midiVelocityGain.setGain (parent->velocityConverter.getGainForVelocity (newGain));

	adsr.noteOff();
	quickRelease.noteOff();

	isQuickFading = ! allowTailOff;

	keyIsDown		   = false;
	playingButReleased = false;
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::clearCurrentNote()
{
	lastReceivedVelocity		 = 0.0f;
	currentAftertouch			 = 0;
	currentlyPlayingNote		 = -1;	// NB: this function does NOT change currentOutputFreq and that is intentional!!
	noteOnTime					 = 0;
	isQuickFading				 = false;
	keyIsDown					 = false;
	playingButReleased			 = false;
	sustainingFromSostenutoPedal = false;
	isPedalPitchVoice			 = false;
	isDescantVoice				 = false;
	isDoubledByAutomatedVoice	 = false;

	if (quickRelease.isActive()) quickRelease.reset();

	quickRelease.noteOn();

	if (adsr.isActive()) adsr.reset();

	setPan (64);

	resetRampedValues();

	noteCleared();
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::setVelocityMultiplier (const float newMultiplier)
{
	midiVelocityGain.setGain (newMultiplier);
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::softPedalChanged (bool isDown)
{
	softPedalMod.setToggle (isDown);
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::aftertouchChanged (const int newAftertouchValue)
{
	currentAftertouch = newAftertouchValue;

	if (parent->isAftertouchGainOn())
	{
		constexpr auto inv127 = 1.0f / 127.0f;

		const auto newWeightedGain = juce::jlimit (0.0f, 1.0f,
												   lastReceivedVelocity + newAftertouchValue * inv127);

		aftertouchGain.setGain (parent->velocityConverter.getGainForVelocity (newWeightedGain));
	}
	else
	{
		aftertouchGain.setGain (1.0f);
	}
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

template <typename SampleType>
void SynthVoiceBase<SampleType>::resetRampedValues()
{
	midiVelocityGain.reset();
	softPedalMod.reset();
	playingButReleasedMod.reset();
	aftertouchGain.reset();
	panner.reset();
}


template <typename SampleType>
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

	playingButReleasedMod.setToggle (playingButReleased);
}


template <typename SampleType>
void SynthVoiceBase<SampleType>::setPan (int newPan)
{
	panner.setMidiPan (juce::jlimit (0, 127, newPan));
}


template class SynthVoiceBase<float>;
template class SynthVoiceBase<double>;

}  // namespace lemons::dsp
