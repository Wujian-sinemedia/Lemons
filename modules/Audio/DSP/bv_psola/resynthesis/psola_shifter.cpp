
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

    if (! areAnyGrainsActive())
        startNewGrain();
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

    if (! areAnyGrainsActive())
        startNewGrain();

    auto sample = SampleType (0);

    for (auto* grain : grains)
    {
        if (grain->isActive())
        {
            sample += grain->getNextSample();

            if (! grain->isActive() || grain->isHalfwayThrough())
                startNewGrain();
        }
    }

    ++currentSample;

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
        const auto zeroes = 0;

        grain->startNewGrain (analyzer.getStartOfClosestGrain (currentSample),
                              analyzer.getGrainLength(),
                              zeroes);
    }
    else
    {
        jassertfalse;
    }
}

template < typename SampleType >
bool PsolaShifter< SampleType >::areAnyGrainsActive() const
{
    for (auto* grain : grains)
        if (grain->isActive())
            return true;

    return false;
}

template < typename SampleType >
psola::RespacedGrain< SampleType >* PsolaShifter< SampleType >::getAvailableGrain() const
{
    for (auto* grain : grains)
        if (! grain->isActive())
            return grain;

    return nullptr;
}


template class PsolaShifter< float >;
template class PsolaShifter< double >;

}  // namespace bav::dsp
