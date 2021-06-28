
namespace bav::dsp::osc::supersaw
{

template<typename SampleType>
void Node<SampleType>::resetPhase()
{
    saw.resetPhase();
}

template<typename SampleType>
void Node<SampleType>::setFrequency (SampleType frequency, SampleType samplerate)
{
    saw.setFrequency (frequency, samplerate);
}

template<typename SampleType>
SampleType Node<SampleType>::getSample()
{
    return saw.getSample();
}

template class Node<float>;
template class Node<double>;

}
