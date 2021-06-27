
namespace bav::dsp
{
template < typename SampleType >
PsolaShifter< SampleType >::PsolaShifter (Analyzer& parentAnalyzer)
    : analyzer (parentAnalyzer), listener (analyzer.getBroadcaster(), [&]
                                           { newBlockStarting(); })
{
    while (grains.size() < 40)
        grains.add (new Grain (analyzer.getStorage()));
}

template < typename SampleType >
void PsolaShifter< SampleType >::newBlockStarting()
{
    currentSample = 0;
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
    jassert (desiredPeriod > 0);

    if (samplesToNextGrain == 0 || ! areAnyGrainsActive())
        startNewGrain();

    auto sample = SampleType (0);

    for (auto* grain : grains)
        if (grain->isActive())
            sample += grain->getNextSample();

    ++currentSample;
    --samplesToNextGrain;

    return sample;
}

template < typename SampleType >
void PsolaShifter< SampleType >::setPitch (float desiredFrequency, double samplerate)
{
    desiredPeriod = math::periodInSamples (samplerate, desiredFrequency);
}

template < typename SampleType >
void PsolaShifter< SampleType >::startNewGrain()
{
    if (auto* grain = getAvailableGrain())
    {
        grain->startNewGrain (analyzer.getStartOfClosestGrain (currentSample),
                              analyzer.getGrainLength());
    }
    else
    {
        jassertfalse;
    }

    samplesToNextGrain = desiredPeriod;
}

template < typename SampleType >
psola::SynthesisGrain< SampleType >* PsolaShifter< SampleType >::getAvailableGrain() const
{
    for (auto* grain : grains)
        if (! grain->isActive())
            return grain;

    return nullptr;
}

template < typename SampleType >
bool PsolaShifter< SampleType >::areAnyGrainsActive() const
{
    for (auto* grain : grains)
        if (grain->isActive())
            return true;
    
    return false;
}


template class PsolaShifter< float >;
template class PsolaShifter< double >;

}  // namespace bav::dsp
