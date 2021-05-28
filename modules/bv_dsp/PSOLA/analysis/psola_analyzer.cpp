
#include "GrainExtractor/GrainExtractor.cpp"
#include "AnalysisGrain/AnalysisGrain.cpp"


namespace bav::dsp
{

template<typename SampleType>
PsolaAnalyzer<SampleType>::PsolaAnalyzer()
{
    pitchDetector.setConfidenceThresh (SampleType (0.3));
    
    pitchDetector.setHzRange (60, 1500);
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::initialize()
{
    pitchDetector.initialize();
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::prepare (int blocksize)
{
    while (analysisGrains.size() < numAnalysisGrains)
        analysisGrains.add (new Analysis_Grain());
    
    for (auto* grain : analysisGrains)
        grain->reserveSize (blocksize);
    
    indicesOfGrainOnsets.ensureStorageAllocated (blocksize);
    grainExtractor.prepare (blocksize);
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::setSamplerate (double sr)
{
    samplerate = sr;
    pitchDetector.setSamplerate (sr);
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::reset()
{
    for (auto* grain : analysisGrains)
        grain->clear();
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::clearUnusedGrains()
{
    for (auto* grain : analysisGrains)
        if (grain->numReferences() <= 0 || grain->isEmpty()) grain->clear();
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::releaseResources()
{
    indicesOfGrainOnsets.clear();
    grainExtractor.releaseResources();
    analysisGrains.clear();
    pitchDetector.releaseResources();
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::analyzeInput (const SampleType* inputSamples, const int numSamples)
{
    jassert (samplerate > 0);
    
    const auto inputFrequency = pitchDetector.detectPitch (inputSamples, numSamples); // outputs 0.0 if frame is unpitched
    const bool frameIsPitched = inputFrequency > 0;
    
    const auto periodThisFrame = frameIsPitched ? math::periodInSamples (samplerate, inputFrequency)
                                                : juce::Random::getSystemRandom().nextInt (pitchDetector.getCurrentLegalPeriodRange());
    
    jassert (analysisGrains.size() == numAnalysisGrains);
    jassert (periodThisFrame > 0 && numSamples >= periodThisFrame * 2);
    
    grainExtractor.getGrainOnsetIndices (indicesOfGrainOnsets, inputSamples, numSamples, periodThisFrame);
    
    const auto grainSize = periodThisFrame * 2;
    
    jassert (!indicesOfGrainOnsets.isEmpty());
    jassert (indicesOfGrainOnsets.getLast() + grainSize <= numSamples);
    
    for (int index : indicesOfGrainOnsets) //  write to analysis grains...
    {
        auto* grain = getEmptyGrain();
        jassert (grain != nullptr);
        grain->storeNewGrain (inputSamples, index, index + grainSize, grainSize);
    }
    
    if (indicesOfGrainOnsets.getUnchecked (0) > 0) // bit @ beginning
        if (auto* grain = getEmptyGrain()) grain->storeNewGrain (inputSamples, 0, indicesOfGrainOnsets.getUnchecked (0), grainSize);
    
    if (indicesOfGrainOnsets.getLast() + grainSize < numSamples) // bit @ end
        if (auto* grain = getEmptyGrain()) grain->storeNewGrain (inputSamples, indicesOfGrainOnsets.getLast() + grainSize, numSamples, grainSize);
    
    lastPeriod = periodThisFrame;
}

template<typename SampleType>
AnalysisGrain< SampleType >* PsolaAnalyzer<SampleType>::findClosestGrain (int idealBufferPos) const
{
    Analysis_Grain* closestGrain = nullptr;
    int             distance     = std::numeric_limits< int >::max();
    
    for (auto* grain : analysisGrains)
    {
        if (grain->isEmpty()) continue;
        
        const auto newDist = abs (idealBufferPos - grain->pitchMark());
        
        if (closestGrain == nullptr || newDist <= distance)
        {
            closestGrain = grain;
            distance     = newDist;
        }
    }
    
    jassert (closestGrain != nullptr);
    
    return closestGrain;
}

template<typename SampleType>
AnalysisGrain< SampleType >* PsolaAnalyzer<SampleType>::findBestNewGrain (Analysis_Grain* prevGrain) const
{
    jassert (prevGrain != nullptr);
    
    Analysis_Grain* newGrain   = nullptr;
    SampleType      difference = 0;
    
    for (auto* grain : analysisGrains)
    {
        if (grain->isEmpty() || grain == prevGrain) continue;
        
        SampleType currentDifference = 0;
        
        const auto numSamples = std::min (prevGrain->getSize(), grain->getSize());
        jassert (numSamples > 0);
        
        for (int i = 0; i < numSamples; ++i)
            currentDifference += abs (prevGrain->getSample (i) - grain->getSample (i));
        
        currentDifference /= numSamples;
        
        if (newGrain == nullptr || currentDifference < difference)
        {
            difference = currentDifference;
            newGrain   = grain;
        }
    }
    
    if (newGrain == nullptr) return prevGrain;
    
    return newGrain;
}

template<typename SampleType>
AnalysisGrain< SampleType >* PsolaAnalyzer<SampleType>::getEmptyGrain() const
{
    for (auto* grain : analysisGrains)
        if (grain->isEmpty()) return grain;
    
    return nullptr;
}

template<typename SampleType>
int PsolaAnalyzer<SampleType>::getLatency() const
{
    return pitchDetector.getLatencySamples();
}

template<typename SampleType>
int PsolaAnalyzer<SampleType>::getLastPeriod() const noexcept
{
    return lastPeriod;
}

template<typename SampleType>
float PsolaAnalyzer<SampleType>::getLastFrequency() const
{
    return static_cast<float> (math::freqFromPeriod (samplerate, lastPeriod));
}


template class PsolaAnalyzer< float >;
template class PsolaAnalyzer< double >;

}  // namespace
