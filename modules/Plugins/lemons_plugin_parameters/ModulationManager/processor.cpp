
namespace lemons::plugin
{
template < typename SampleType >
ModulationManagerProcessor< SampleType >::ModulationManagerProcessor (ModulationManager& managerToUse)
    : manager (managerToUse)
{
}

template < typename SampleType >
void ModulationManagerProcessor< SampleType >::prepareToPlay (double samplerate, int blocksize)
{
    for (auto* lfo : manager.lfos)
        lfo->prepareToPlay (blocksize, samplerate);

    midiStorage.ensureSize (static_cast< size_t > (blocksize));
}

template < typename SampleType >
void ModulationManagerProcessor< SampleType >::processBlock (AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    const auto numSamples = audio.getNumSamples();

    for (auto* lfo : manager.lfos)
        lfo->processNextBlock (numSamples);

    const auto blockSize = manager.blocksize;

    int blockStart = 0;

    while (blockStart + blockSize < numSamples)
    {
        AudioBuffer< SampleType > alias {audio.getArrayOfWritePointers(), audio.getNumChannels(), blockStart, blockSize};

        midi::copyRangeOfMidiBuffer (midi, midiStorage, blockStart, 0, blockSize);

        processInternal (alias, midiStorage);

        midi::copyRangeOfMidiBuffer (midiStorage, midi, 0, blockStart, blockSize);

        blockStart += blockSize;

        for (auto* lfo : manager.lfos)
            lfo->advance (blockSize);
    }

    if (blockStart < numSamples)
    {
        const auto chunkSamples = numSamples - blockStart;

        AudioBuffer< SampleType > alias {audio.getArrayOfWritePointers(), audio.getNumChannels(), blockStart, chunkSamples};

        midi::copyRangeOfMidiBuffer (midi, midiStorage, blockStart, 0, chunkSamples);

        processInternal (alias, midiStorage);

        midi::copyRangeOfMidiBuffer (midiStorage, midi, 0, blockStart, chunkSamples);
    }
}

template < typename SampleType >
void ModulationManagerProcessor< SampleType >::processInternal (AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    // loop thru all parameters & update them w current lfo vals

    renderChunk (audio, midi);
}


template class ModulationManagerProcessor< float >;
template class ModulationManagerProcessor< double >;

}  // namespace bav::plugin
