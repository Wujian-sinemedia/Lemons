
namespace lemons::plugin
{
ModulationManager::LFO::LFO (ParameterList& listToUse, String metaParameterName)
: param (listToUse, metaParameterName)
{
}

void ModulationManager::LFO::prepareToPlay (int numSamples, double samplerate)
{
    storage.setSize (1, numSamples);
    osc.prepare (numSamples, samplerate);
}

void ModulationManager::LFO::prepareNextBlock (int numSamples)
{
    currentTick = 0;

    storage.clear();

    auto alias = dsp::buffers::getAliasBuffer (storage, 0, numSamples, 1);

    osc.process (alias);
}

void ModulationManager::LFO::processNextSample (bool advance)
{
    const auto sample = 0.f;

    getParameter().set (juce::roundToInt (juce::jmap (sample, -1.f, 1.f, 0.f, 100.f)));

    if (advance)
        currentTick++;
}

[[nodiscard]] ModulationManager::LFO::Osc& ModulationManager::LFO::getOscillator() noexcept
{
    return osc;
}

[[nodiscard]] DefaultMetaParameter& ModulationManager::LFO::getParameter() noexcept
{
    return *param.get();
}

void ModulationManager::LFO::addParameterTo (juce::AudioProcessor& processor)
{
    param.addTo (processor);
}

void ModulationManager::LFO::serialize (TreeReflector& ref)
{
    ref.add ("Oscillator", osc);
    ref.add ("MetaParameter", param);
}

}  // namespace lemons::plugin
