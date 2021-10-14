
namespace lemons::plugin
{
ModulationManager::LFO::LFO (ParameterList& listToUse)
    : paramList (&listToUse)
{
}

void ModulationManager::LFO::setParamList (ParameterList& listToUse)
{
    paramList = &listToUse;

    for (auto* con : connections)
        con->paramList = paramList;
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

    for (auto* con : connections)
        con->apply (sample);

    if (advance)
        currentTick++;
}

[[nodiscard]] dsp::osc::ChoosableOscillator< float >& ModulationManager::LFO::getOscillator() noexcept
{
    return osc;
}

void ModulationManager::LFO::serialize (TreeReflector& ref)
{
    if (ref.isSaving())
        removeInvalidConnections();

    ref.add ("Oscillator", osc);
    ref.add ("Connections", connections);
}

[[nodiscard]] bool ModulationManager::LFO::hasConnection (Parameter& parameter) const
{
    for (const auto* con : connections)
        if (con->param == &parameter)
            return true;

    return false;
}

[[nodiscard]] int ModulationManager::LFO::getConnectionAmount (Parameter& parameter) const
{
    for (const auto* con : connections)
        if (con->param == &parameter)
            return con->percentEffect;

    return -1;
}

void ModulationManager::LFO::createOrEditConnection (Parameter& parameter, int amount)
{
    jassert (parameter.isAutomatable());

    Connection* toUse = nullptr;

    for (auto* con : connections)
    {
        if (con->param == &parameter)
        {
            con->percentEffect = amount;
            return;
        }

        if (con->param == nullptr)
            toUse = con;
    }

    if (toUse != nullptr)
    {
        toUse->param         = &parameter;
        toUse->percentEffect = amount;
    }
    else
    {
        jassert (paramList != nullptr);
        connections.add (new Connection (*paramList, parameter, amount));
    }
}

void ModulationManager::LFO::removeConnection (Parameter& parameter)
{
    for (auto* con : connections)
    {
        if (con->param == &parameter)
        {
            con->param = nullptr;
            return;
        }
    }
}

void ModulationManager::LFO::removeInvalidConnections()
{
    for (auto* con : connections)
        if (con->param == nullptr)
            connections.removeObject (con);
}


/*------------------------------------------------------------------------*/

ModulationManager::LFO::Connection::Connection (ParameterList& listToUse, Parameter& parameter, int amount)
    : param (&parameter), percentEffect (amount), paramList (&listToUse)
{
}

void ModulationManager::LFO::Connection::serialize (TreeReflector& ref)
{
    auto loadParameter = [&] (String& name)
    {
        if (param->getParameterName() == name) return;
        param = nullptr;
        if (paramList == nullptr) return;
        param = paramList->getParameterWithName (name);
    };

    ref.addLambdaSet< String > (
        "ParameterName",
        [&]
        { return param->getParameterName(); },
        loadParameter);

    ref.add ("Amount", percentEffect);
}

void ModulationManager::LFO::Connection::apply (float newLFOsample) const
{
    if (param == nullptr)
        return;

    const auto before = param->getNormalizedValue();

    const auto diff = newLFOsample / (float) percentEffect;

    const auto newSample = (before + diff) * 0.5f;

    param->setNormalizedValue (newSample);
}


}  // namespace lemons::plugin
