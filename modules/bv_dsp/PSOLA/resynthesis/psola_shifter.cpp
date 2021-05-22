
#include "SynthesisGrain/SynthesisGrain.cpp"


namespace bav::dsp
{

template<typename SampleType>
PsolaShifter<SampleType>::PsolaShifter (Analyzer& parentAnalyzer)
: analyzer (parentAnalyzer)
{
    jassert (analyzer != nullptr);
}


template<typename SampleType>
void PsolaShifter<SampleType>::prepare()
{
    while (synthesisGrains.size() < numSynthesisGrains)
        synthesisGrains.add (new Synthesis_Grain());
}

template<typename SampleType>
void PsolaShifter<SampleType>::newBlockComing (int prevBlocksize) noexcept
{
    //    nextSynthesisPitchMark = std::max(0, nextSynthesisPitchMark - prevBlocksize);
    
    juce::ignoreUnused (prevBlocksize);
    
    nextAnalysisPitchMark  = 0;
    nextSynthesisPitchMark = 0;
    
    //nextSynthesisPitchMark -= prevBlocksize;
}

template<typename SampleType>
void PsolaShifter<SampleType>::reset()
{
    //       nextSynthesisPitchMark = 0;
    
    for (auto* grain : synthesisGrains)
        grain->stop();
}

template<typename SampleType>
void PsolaShifter<SampleType>::releaseResources()
{
    nextSynthesisPitchMark = 0;
    synthesisGrains.clear();
}

template<typename SampleType>
void PsolaShifter<SampleType>::bypassedBlockRecieved (int numSamples)
{
    for (auto* grain : synthesisGrains)
        grain->stop();
    
    juce::ignoreUnused (numSamples);
    
    //       nextSynthesisPitchMark = std::max(0, nextSynthesisPitchMark - numSamples);
}

template<typename SampleType>
void PsolaShifter<SampleType>::getSamples (SampleType* outputSamples, const int numSamples, const int newPeriod)
{
    const auto origPeriod = analyzer.getLastPeriod();
    
    for (int i = 0; i < numSamples; ++i)
        outputSamples[i] = getNextSample (newPeriod, origPeriod);
}

template<typename SampleType>
SampleType PsolaShifter<SampleType>::getNextSample (const int newPeriod, const int origPeriod)
{
    jassert (synthesisGrains.size() == numSynthesisGrains);
    jassert (newPeriod > 0 && origPeriod > 0);
    
    if (!anyGrainsAreActive()) startNewGrain (newPeriod, origPeriod, nullptr);
    
    auto sample = SampleType (0);
    
    for (auto* grain : synthesisGrains)
    {
        if (!grain->isActive()) continue;
        
        sample += grain->getNextSample();
        
        if (!grain->isActive() || grain->isHalfwayThrough()) startNewGrain (newPeriod, origPeriod, grain->orig());
    }
    
    //        if (nextSynthesisPitchMark > 0)
    //            --nextSynthesisPitchMark;
    
    return sample;
}

template<typename SampleType>
void PsolaShifter<SampleType>::startNewGrain (const int newPeriod, const int origPeriod, AnalysisGrain< SampleType >* lastGrain)
{
    juce::ignoreUnused (lastGrain);
    
    if (auto* newGrain = getAvailableGrain())
    {
        //           auto* analysisGrain = lastGrain == nullptr ? analyzer->findClosestGrain (bufferPos) : analyzer->findBestNewGrain (lastGrain);
        
        //           const auto bufferPos = anyGrainsAreActive() ? lastAnalysisPitchMark + origPeriod : 0;
        
        auto* analysisGrain = analyzer.findClosestGrain (nextAnalysisPitchMark);
        
        const auto samplesInFuture = juce::roundToInt (analysisGrain->percentOfExpectedSize() * (nextSynthesisPitchMark - nextAnalysisPitchMark));
        
        newGrain->startNewGrain (analysisGrain, samplesInFuture);
        
        nextSynthesisPitchMark += newPeriod;
        nextAnalysisPitchMark += origPeriod;
    }
}

template<typename SampleType>
SynthesisGrain< SampleType >* PsolaShifter<SampleType>::getAvailableGrain()
{
    for (auto* grain : synthesisGrains)
        if (!grain->isActive()) return grain;
    
    return nullptr;
    
    //        Synthesis_Grain* newGrain = nullptr;
    //        int mostZeroesLeft = 0;
    //
    //        for (auto* grain : synthesisGrains)
    //        {
    //            const auto zeroesLeft = grain->silenceLeft();
    //
    //            if (newGrain == nullptr || zeroesLeft > mostZeroesLeft)
    //            {
    //                newGrain = grain;
    //                mostZeroesLeft = zeroesLeft;
    //            }
    //        }
    //
    //        if (mostZeroesLeft == 0)
    //            return nullptr;
    //
    //        return newGrain;
}

template<typename SampleType>
bool PsolaShifter<SampleType>::anyGrainsAreActive() const
{
    for (auto* grain : synthesisGrains)
        if (grain->isActive()) return true;
    
    return false;
}


template class PsolaShifter< float >;
template class PsolaShifter< double >;

}  // namespace
