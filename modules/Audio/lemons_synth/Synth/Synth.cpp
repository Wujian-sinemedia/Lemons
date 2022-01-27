
namespace lemons::dsp
{
template <typename SampleType>
void SynthBase<SampleType>::initialize (int initNumVoices, double initSamplerate, int initBlocksize)
{
	jassert (initNumVoices > 0 && initSamplerate > 0 && initBlocksize > 0);

	changeNumVoices (initNumVoices);

	initialized (initSamplerate, initBlocksize);

	prepare (initSamplerate, initBlocksize);
}

template <typename SampleType>
bool SynthBase<SampleType>::isInitialized() const
{
	return getNumVoices() > 0 && sampleRate > 0;
}


/*
 Immediately clears all notes & resets the synth's internal state.
 */
template <typename SampleType>
void SynthBase<SampleType>::reset()
{
	for (auto* voice : voices)
	{
		voice->clearCurrentNote();
		voice->resetRampedValues();
	}

	panner.reset();

	resetTriggered();
}


/*
 Prepares the synth for a new expected maximum blocksize.
 */
template <typename SampleType>
void SynthBase<SampleType>::prepare (double samplerate, int blocksize)
{
	jassert (blocksize > 0 && samplerate > 0);
	jassert (! voices->isEmpty());

	aggregateMidiBuffer.ensureSize (static_cast<size_t> (blocksize * 2));
	aggregateMidiBuffer.clear();

	for (auto* voice : voices)
		voice->prepare (samplerate, blocksize);

	panner.prepare (voices->size(), false);
	midi.prepare (blocksize);

	sampleRate = samplerate;

	prepared (samplerate, blocksize);
}


/*
 Attempts to reduce the synth's footprint as much as possible.
 After calling this, you must call initialize() again before attempting to render any more audio with the synth.
 */
template <typename SampleType>
void SynthBase<SampleType>::releaseResources()
{
	aggregateMidiBuffer.clear();
	currentNotes.clear();
	desiredNotes.clear();

	for (auto* voice : voices)
		voice->released();

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
 Internally, the synth will render smaller chunks of audio in between each midi message timestamp, to keep midi events synchronous with the audio output. If you are implementing a class that derives from this class or the SynthVoiceBase class, note that the SynthVoiceBase's renderPlease() method may receive blocksizes as small as 1 sample (but never 0 samples).
 */
template <typename SampleType>
void SynthBase<SampleType>::renderVoices (MidiBuffer& midiMessages, AudioBuffer<SampleType>& output)
{
	jassert (! voices->isEmpty());
	jassert (sampleRate > 0);

	output.clear();
	aggregateMidiBuffer.clear();

	const auto numSamples = output.getNumSamples();

	midiInputStorage.addEvents (midiMessages, 0, numSamples, 0);

	for (auto* voice : voices)
		voice->newBlockComing (lastBlocksize, numSamples);

	lastBlocksize = numSamples;

	midi.process (output, midiMessages);

	midiMessages.swapWith (aggregateMidiBuffer);
	keyboardState.processNextMidiBuffer (aggregateMidiBuffer, 0, numSamples, false);

	aggregateMidiBuffer.clear();
	midiInputStorage.clear();
}

/*
 This should be called when a block is received while the processor is in bypassed mode.
 This will not output any audio, but may alter the contents of the MidiBuffer.
 */
template <typename SampleType>
void SynthBase<SampleType>::bypassedBlock (int numSamples, MidiBuffer& midiMessages)
{
	//    midi.processBypassed (midiMessages);

	for (auto* voice : voices)
		voice->bypassedBlock (numSamples);
}


/*
 Call this function to change the number of voices the synth is managing.
 Note that this function will allocate!
 */
template <typename SampleType>
void SynthBase<SampleType>::changeNumVoices (int newNumVoices)
{
	voices.resize (newNumVoices);

	jassert (voices->size() == newNumVoices);

	numVoicesChanged();
}


/*
 Removes a specified number of voices from the synth. This function attempts to remove inactive voices first.
 */
// template <typename SampleType>
// void SynthBase<SampleType>::removeNumVoices (int voicesToRemove)
//{
//    if (voicesToRemove == 0) return;
//
//#if JUCE_DEBUG
//    const auto shouldBeLeft = voices.size() - voicesToRemove;
//#endif
//
//    int voicesRemoved = 0;
//
//    while (voicesRemoved < voicesToRemove && ! voices.isEmpty())
//    {
//        if (voices.isEmpty()) break;
//
//        Voice* removing = voiceAllocator.findFreeVoice (true);
//
//        if (removing == nullptr) removing = voices[0];
//
//        if (removing->isVoiceActive())
//        {
//            panner.panValTurnedOff (removing->getCurrentMidiPan());
//            aggregateMidiBuffer.addEvent (MidiMessage::noteOff (removing->getMidiChannel(), removing->getCurrentlyPlayingNote(), 1.0f),
//                                          midi.router.getLastMidiTimestamp());
//        }
//
//        voices.removeObject (removing, true);
//
//        ++voicesRemoved;
//    }
//
//    jassert (voices.isEmpty() || voices.size() == shouldBeLeft);
//
//    numVoicesChanged();
// }


/*
 This function should be called any time the synth's number of voices is changed.
 */
template <typename SampleType>
void SynthBase<SampleType>::numVoicesChanged()
{
	const auto newMaxNumVoices = voices->size();

	panner.prepare (newMaxNumVoices, false);
	voiceAllocator.prepare (newMaxNumVoices);

	currentNotes.ensureStorageAllocated (newMaxNumVoices);
	desiredNotes.ensureStorageAllocated (newMaxNumVoices);
}


/*
 Returns a pointer to the voice playing a certain note. This will return nullptr if the note is not currently active.
 */
template <typename SampleType>
SynthVoiceBase<SampleType>* SynthBase<SampleType>::getVoicePlayingNote (int midiPitch) const
{
	auto it = std::find_if (voices.begin(),
							voices.end(),
							[&midiPitch] (SynthVoiceBase<SampleType>* voice)
							{ return voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == midiPitch; });

	return it != voices.end() ? *it : nullptr;
}


/*
 Returns the number of currently active voices.
 Voices in any state are counted - including playing-but-released, sustaining from sostenuto pedal, etc.
 */
template <typename SampleType>
int SynthBase<SampleType>::getNumActiveVoices() const
{
	return static_cast<int> (
		std::count_if (voices.begin(), voices.end(), [] (SynthVoiceBase<SampleType>* voice)
					   { return voice->isVoiceActive(); }));
}


template class SynthBase<float>;
template class SynthBase<double>;


}  // namespace lemons::dsp
