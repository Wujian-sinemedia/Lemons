
namespace lemons::plugin
{
ModulationManager::LFO::LFO (ParameterList& listToUse)
    : paramList (&listToUse)
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

    AudioBuffer< float > alias {storage.getArrayOfWritePointers(), 1, 0, numSamples};

    osc.process (alias);
}

float ModulationManager::LFO::getAndDontAdvance() const
{
    return storage.getSample (0, currentTick);
}

float ModulationManager::LFO::getAndAdvance()
{
    return storage.getSample (0, currentTick++);
}

void ModulationManager::LFO::advance (int numTicks)
{
    currentTick += numTicks;
}

void ModulationManager::LFO::processNextSample (bool advance)
{
    const auto sample = getAndDontAdvance();

    for (auto* con : connections)
        con->apply (sample);

    if (advance)
        currentTick++;
}

dsp::osc::ChoosableOscillator< float >& ModulationManager::LFO::getOscillator()
{
    return osc;
}

void ModulationManager::LFO::serialize (TreeReflector& ref)
{
    if (ref.isSaving())
        removeInvalidConnections();

    ref.add ("Oscillator", osc);
    ref.add ("Connection", connections);
}

bool ModulationManager::LFO::hasConnection (Parameter& parameter) const
{
    for (const auto* con : connections)
    {
        if (con->param == nullptr) continue;

        if (con->param == &parameter)
            return true;
    }

    return false;
}

void ModulationManager::LFO::createOrEditConnection (Parameter& parameter, int amount)
{
    Connection* toUse = nullptr;

    for (auto* con : connections)
    {
        if (con->param == nullptr)
        {
            toUse = con;
            continue;
        }

        if (con->param == &parameter)
        {
            con->percentEffect = amount;
            return;
        }
    }

    if (toUse != nullptr)
    {
        toUse->param         = &parameter;
        toUse->percentEffect = amount;
        return;
    }

    jassert (paramList != nullptr);
    connections.add (new Connection (*paramList, parameter, amount));
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


/*---------------------------------------------------------------------------------------------------*/

//ModulationManager::ModulationManager (ParameterList& listToUse)
//: paramList(listToUse)
//{
//
//}

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

}  // namespace lemons::plugin
