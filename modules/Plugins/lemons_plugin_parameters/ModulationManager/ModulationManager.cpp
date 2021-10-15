
namespace lemons::plugin
{

static inline std::function< String (int) > createDefaultLfoNamingFunc()
{
    return [] (int idx) -> String
    {
        return TRANS ("LFO") + " " + String (idx);
    };
}

ModulationManager::ModulationManager (ParameterList& listToUse,
                                      int            initNumLfos,
                                      LfoNamingFunc  namingFuncToUse)
    : paramList (listToUse), namingFunc (namingFuncToUse)
{
    if (namingFunc == nullptr)
        namingFunc = createDefaultLfoNamingFunc();

    while (lfos.size() < initNumLfos)
        addLFO();
}

LFO& ModulationManager::addLFO()
{
    auto* lfo = lfos.add (new LFO (paramList, namingFunc (lfos.size())));
    return *lfo;
}

void ModulationManager::serialize (TreeReflector& ref)
{
    using State = std::vector< ValueTree >;

    auto getState = [&]() -> State
    {
        State state;

        for (auto* lfo : lfos)
            state.push_back (lfo->SerializableData::serialize());

        return state;
    };

    auto setState = [&] (State& state)
    {
        for (auto i = static_cast< int > (state.size()); i < lfos.size(); ++i)
            lfos.remove (i);

        for (auto i = static_cast< State::size_type > (lfos.size()); i < state.size(); ++i)
        {
            addLFO().deserialize (state[i]);
        }
    };

    ref.addLambdaSet< State > ("LFOs", getState, setState);
}

LFO* ModulationManager::getLFO (int index)
{
    if (index >= lfos.size()) return nullptr;
    return lfos[index];
}

LFO* ModulationManager::getLFOofType (dsp::osc::OscType type)
{
    for (auto* lfo : lfos)
        if (lfo->getOscillator().getOscType() == type)
            return lfo;

    return nullptr;
}

LFO* ModulationManager::getLFOatFrequency (float freq)
{
    for (auto* lfo : lfos)
        if (lfo->getOscillator().getFrequency() == freq)
            return lfo;

    return nullptr;
}

LFO* ModulationManager::getLFOwithConnection (Parameter& param)
{
    for (auto* lfo : lfos)
        if (lfo->getParameter().hasConnection (param))
            return lfo;

    return nullptr;
}

void ModulationManager::addAllParametersTo (juce::AudioProcessor& processor)
{
    for (auto* lfo : lfos)
        lfo->addParameterTo (processor);
}

void ModulationManager::addAllParametersAsInternal()
{
    addAllParametersTo (dummyProcessor);
}

}  // namespace lemons::plugin
