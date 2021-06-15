
namespace bav::dsp
{
template < typename SampleType >
PsolaShifter< SampleType >::PsolaShifter (Analyzer& parentAnalyzer)
    : analyzer (parentAnalyzer)
{
}

template < typename SampleType >
void PsolaShifter< SampleType >::getSamples (SampleType* outputSamples, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        outputSamples[i] = getNextSample();
}

template < typename SampleType >
SampleType PsolaShifter< SampleType >::getNextSample()
{
    auto sample = SampleType (0);
    
    return sample;
}

template < typename SampleType >
void PsolaShifter< SampleType >::setPitch (float desiredFrequency, double samplerate)
{
    desiredPeriod = math::periodInSamples (samplerate, desiredFrequency);
}


template class PsolaShifter< float >;
template class PsolaShifter< double >;

}  // namespace bav::dsp
