namespace New
{

template <typename SampleType>
void Engine<SampleType>::renderBlock (const AudioBuffer&, AudioBuffer&, MidiBuffer&, bool)
{
}

template <typename SampleType>
void Engine<SampleType>::prepared (int, double, int)
{
}

template class Engine<float>;
template class Engine<double>;

}  // namespace New
