
namespace lemons::midi
{
template <typename SampleType>
void MidiChoppingProcessor<SampleType>::prepare (int maxBlocksize)
{
	midiStorage.ensureSize (static_cast<size_t> (maxBlocksize));
	dummyBuffer.setSize (1, maxBlocksize, true, true, true);
}

template <typename SampleType>
void MidiChoppingProcessor<SampleType>::processBypassed (int numSamples, MidiBuffer& midi)
{
	dummyBuffer.clear();

	AudioBuffer alias { dummyBuffer.getArrayOfWritePointers(), 1, 0, numSamples };

	process (alias, midi);
}

template <typename SampleType>
void MidiChoppingProcessor<SampleType>::process (AudioBuffer& audio, MidiBuffer& midi)
{
	auto samplesLeft = audio.getNumSamples();

	if (samplesLeft == 0 || audio.getNumChannels() == 0)
	{
		processInternal (audio, midi, 0, 0);
		return;
	}

	auto midiIterator = midi.findNextSamplePosition (0);
	int  startSample  = 0;

	for (; samplesLeft > 0; ++midiIterator)
	{
		if (midiIterator == midi.cend())
		{
			processInternal (audio, midi, startSample, samplesLeft);
			return;
		}

		const auto metadata                 = *midiIterator;
		const auto samplesToNextMidiMessage = metadata.samplePosition - startSample;
		const auto nextMidiMessage          = metadata.getMessage();

		if (samplesToNextMidiMessage >= samplesLeft)
		{
			processInternal (audio, midi, startSample, samplesLeft);
			handleMidiMessage (nextMidiMessage);
			break;
		}

		if (samplesToNextMidiMessage == 0)
		{
			handleMidiMessage (nextMidiMessage);
			continue;
		}

		handleMidiMessage (nextMidiMessage);
		processInternal (audio, midi, startSample, samplesToNextMidiMessage);

		startSample += samplesToNextMidiMessage;
		samplesLeft -= samplesToNextMidiMessage;
	}

	std::for_each (
	    midiIterator, midi.cend(), [&] (const juce::MidiMessageMetadata& meta)
	    { handleMidiMessage (meta.getMessage()); });
}

template <typename SampleType>
void MidiChoppingProcessor<SampleType>::processInternal (AudioBuffer& audio, MidiBuffer& midi,
                                                         int startSample, int numSamples)
{
	AudioBuffer alias { audio.getArrayOfWritePointers(),
		                audio.getNumChannels(),
		                startSample,
		                numSamples };

	copyRangeOfMidiBuffer (midi, midiStorage, startSample, 0, numSamples);

	renderChunk (alias, midiStorage);

	copyRangeOfMidiBuffer (midiStorage, midi, 0, startSample, numSamples);
}


template class MidiChoppingProcessor<float>;
template class MidiChoppingProcessor<double>;

}  // namespace lemons::midi
