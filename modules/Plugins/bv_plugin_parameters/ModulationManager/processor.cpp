
namespace bav::plugin
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
}

template < typename SampleType >
void ModulationManagerProcessor< SampleType >::processBlock (AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    const auto numSamples = audio.getNumSamples();

    for (auto* lfo : manager.lfos)
        lfo->processNextBlock (numSamples);

    for (int s = 0; s < numSamples; ++s)
    {
        // loop thru all parameters & update them w current lfo vals

        // process 1 sample at a time

        for (auto* lfo : manager.lfos)
            lfo->advance();
    }
}


template class ModulationManagerProcessor< float >;
template class ModulationManagerProcessor< double >;

}  // namespace bav::plugin
