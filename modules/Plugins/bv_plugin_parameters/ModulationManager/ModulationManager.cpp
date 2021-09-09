
namespace bav::plugin
{

template<typename SampleType>
void ModulationManager<SampleType>::LFO::prepareToPlay (int numSamples, double samplerate)
{
    storage.setSize (1, numSamples);
    this->prepare (numSamples, samplerate);
}

template<typename SampleType>
void ModulationManager<SampleType>::LFO::processNextBlock (int numSamples)
{
    currentTick = 0;
    
    storage.clear();
    
    AudioBuffer<SampleType> alias {storage.getArrayOfWritePointers(), 1, 0, numSamples};
    
    this->process (alias);
}

template<typename SampleType>
SampleType ModulationManager<SampleType>::LFO::getAndDontAdvance() const
{
    return storage.getSample (0, currentTick);
}

template<typename SampleType>
SampleType ModulationManager<SampleType>::LFO::getAndAdvance()
{
    return storage.getSample (0, currentTick++);
}


/*---------------------------------------------------------------------------------------------------*/

template<typename SampleType>
void ModulationManager<SampleType>::serialize (TreeReflector& ref)
{
    ref.add ("LFO", lfos);
}


template class ModulationManager<float>;
template class ModulationManager<double>;

}
