/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


namespace lemons::midi
{

template <typename SampleType>
void ChoppingProcessor<SampleType>::prepare (int maxBlocksize)
{
	midiStorage.ensureSize (static_cast<size_t> (maxBlocksize));
}

template <typename SampleType>
void ChoppingProcessor<SampleType>::releaseResources()
{
	midiStorage.clear();
}

template <typename SampleType>
bool ChoppingProcessor<SampleType>::shouldChopAroundMidiMessage (const MidiMessage&) const
{
	return true;
}

template <typename SampleType>
void ChoppingProcessor<SampleType>::process (AudioBuffer<SampleType>& audio, MidiBuffer& midi)
{
	const auto findNextMessageToChopAround = [this, &midi] (juce::MidiBufferIterator it) -> juce::MidiBufferIterator
	{
		for (; it != midi.cend(); ++it)
			if (shouldChopAroundMidiMessage ((*it).getMessage()))
				return it;

		return midi.cend();
	};

	const auto numSamples = audio.getNumSamples();

	auto it			  = findNextMessageToChopAround (midi.cbegin());
	int	 lastChunkEnd = 0;

	while (it != midi.cend())
	{
		const auto meta		 = *it;
		const auto message	 = meta.getMessage();
		const auto timestamp = meta.samplePosition;

		jassert (shouldChopAroundMidiMessage (message));

		if (timestamp > lastChunkEnd)
			processInternal (audio, midi, lastChunkEnd, timestamp);

		handleMidiMessage (message);

		it			 = findNextMessageToChopAround (midi.findNextSamplePosition (timestamp + 1));
		lastChunkEnd = it == midi.cend() ? numSamples : (*it).samplePosition;

		if (lastChunkEnd >= numSamples)
		{
			processInternal (audio, midi, timestamp, numSamples);
			break;
		}

		processInternal (audio, midi, timestamp, lastChunkEnd);
	}

	if (lastChunkEnd < numSamples)
	{
		processInternal (audio, midi, lastChunkEnd, numSamples);
		it = findNextMessageToChopAround (midi.findNextSamplePosition (numSamples));
	}

	std::for_each (it, midi.cend(), [this] (const juce::MidiMessageMetadata& meta)
		{
	const auto message = meta.getMessage();

	if (shouldChopAroundMidiMessage (message))
	handleMidiMessage (message); });
}

template <typename SampleType>
void ChoppingProcessor<SampleType>::processInternal (AudioBuffer<SampleType>& audio, MidiBuffer& midi,
	int startSample, int endSample)
{
	const auto numSamples = endSample - startSample;

	if (numSamples == 0)
		return;

	jassert (numSamples > 0);

	const ScopedMidiBufferAlias midiAlias { midi, midiStorage, startSample, numSamples };

	AudioBuffer<SampleType> audioAlias { audio.getArrayOfWritePointers(), audio.getNumChannels(), startSample, numSamples };

	renderChunk (audioAlias, midiStorage);
}


template class ChoppingProcessor<float>;
template class ChoppingProcessor<double>;

}  // namespace lemons::midi
