namespace New
{
template < typename SampleType >
Engine< SampleType >::Engine (State& stateToUse)
    : state (stateToUse)
{
}

template < typename SampleType >
void Engine< SampleType >::renderBlock (const AudioBuffer&, AudioBuffer&, MidiBuffer&, bool)
{
}

template < typename SampleType >
void Engine< SampleType >::prepared (int, double)
{
}

template class Engine< float >;
template class Engine< double >;

}  // namespace New
