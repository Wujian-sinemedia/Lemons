
namespace bav::plugin
{
void ModulationManager::LFO::prepareToPlay (int numSamples, double samplerate)
{
    storage.setSize (1, numSamples);
    this->prepare (numSamples, samplerate);
}

void ModulationManager::LFO::processNextBlock (int numSamples)
{
    currentTick = 0;

    storage.clear();

    AudioBuffer< float > alias {storage.getArrayOfWritePointers(), 1, 0, numSamples};

    this->process (alias);
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


/*---------------------------------------------------------------------------------------------------*/

void ModulationManager::setBlocksize (int newBlocksize)
{
    blocksize = newBlocksize;
}

void ModulationManager::serialize (TreeReflector& ref)
{
    ref.add ("LFO", lfos);
    ref.add ("Blocksize", blocksize);
}

}  // namespace bav::plugin
