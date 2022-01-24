
namespace lemons::dsp::synth
{
template <typename SampleType>
VoiceAllocator<SampleType>::VoiceAllocator (SynthBase<SampleType>& s)
    : synth (s)
{
}

template <typename SampleType>
void VoiceAllocator<SampleType>::prepare (int maxNumVoices)
{
	usableVoices.clearQuick();
	usableVoices.ensureStorageAllocated (maxNumVoices);
}

/*
 Attempts to find the appropriate voice to start when a note-on event is received.
 */
template <typename SampleType>
SynthVoiceBase<SampleType>* VoiceAllocator<SampleType>::findFreeVoice (bool stealIfNoneAvailable)
{
	jassert (! synth.voices->isEmpty());

	/* Look into the future!  If a voice has a note off coming within the next few milliseconds, let's steal that voice... */
	constexpr int futureStealingMaxMs = 10;

	for (auto* voice : synth.voices)
	{
		if (! voice->isVoiceActive()) continue;

		const auto note = voice->getCurrentlyPlayingNote();

		for (auto midiIterator = synth.midiInputStorage.findNextSamplePosition (synth.midi.router.getLastMidiTimestamp() + 1);
		     midiIterator != synth.midiInputStorage.findNextSamplePosition (synth.midi.router.getLastMidiTimestamp() + futureStealingMaxMs + 1);
		     ++midiIterator)
		{
			const auto metadata = *midiIterator;
			const auto msg      = metadata.getMessage();

			if (msg.isNoteOff() && msg.getNoteNumber() == note) return voice;
		}
	}

	for (auto* voice : synth.voices)
		if (! voice->isVoiceActive()) return voice;

	if (stealIfNoneAvailable) return findVoiceToSteal();

	return nullptr;
}


/*
 If findFreeVoice() is called and every voice is active, this function will attempt to find the optimal voice to "steal" for the new note.
 This voice stealing algorithm protects the highest & lowest notes whose keyboard keys are still being held, and the pedal & descant voices, if they're active.
 */
template <typename SampleType>
SynthVoiceBase<SampleType>* VoiceAllocator<SampleType>::findVoiceToSteal()
{
	// These are the voices we want to protect
	Voice* low = nullptr;  // Lowest sounding note, might be sustained, but NOT in release phase
	Voice* top = nullptr;  // Highest sounding note, might be sustained, but NOT in release phase

	// protect these, only use if necessary. These will be nullptrs if pedal / descant is currently off
	auto* descantVoice = synth.descant.getVoice();
	auto* pedalVoice   = synth.pedal.getVoice();

	usableVoices.clearQuick();  // this is a list of voices we can steal, sorted by how long they've been on

	for (auto* voice : synth.voices)
	{
		if (voice == descantVoice || voice == pedalVoice) continue;

		usableVoices.add (voice);

		std::sort (usableVoices.begin(), usableVoices.end(),
		           [] (const Voice* a, const Voice* b)
		           { return a->wasStartedBefore (*b); });

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


template class VoiceAllocator<float>;
template class VoiceAllocator<double>;

}  // namespace lemons::dsp::synth
