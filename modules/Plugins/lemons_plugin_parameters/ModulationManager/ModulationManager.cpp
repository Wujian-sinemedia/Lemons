
namespace lemons::plugin
{
ModulationManager::ModulationManager (ParameterList& listToUse)
    : paramList (listToUse)
{
}

void ModulationManager::prepare (int blocksize, double samplerate)
{
    for (auto* lfo : lfos)
        lfo->prepareToPlay (blocksize, samplerate);
}

void ModulationManager::processNextChunk (int numSamples)
{
    for (auto* lfo : lfos)
    {
        lfo->prepareNextBlock (numSamples);
        lfo->processNextSample();
    }
}

void ModulationManager::serialize (TreeReflector& ref)
{
    ref.add ("LFO", lfos);

    if (ref.isLoading())
        for (auto* lfo : lfos)
            lfo->setParamList (paramList);
}

ModulationManager::LFO* ModulationManager::getLFO (int index)
{
    if (index > lfos.size()) return nullptr;
    return lfos[index];
}

ModulationManager::LFO* ModulationManager::getLFOofType (dsp::osc::OscType type)
{
    for (auto* lfo : lfos)
        if (lfo->getOscillator().getOscType() == type)
            return lfo;

    return nullptr;
}

ModulationManager::LFO* ModulationManager::getLFOatFrequency (float freq)
{
    for (auto* lfo : lfos)
        if (lfo->getOscillator().getFrequency() == freq)
            return lfo;

    return nullptr;
}

ModulationManager::LFO* ModulationManager::getLFOwithConnection (Parameter& param)
{
    for (auto* lfo : lfos)
        if (lfo->hasConnection (param))
            return lfo;

    return nullptr;
}

void ModulationManager::removeAllInvalidConnections()
{
    for (auto* lfo : lfos)
        lfo->removeInvalidConnections();
}

}  // namespace lemons::plugin
