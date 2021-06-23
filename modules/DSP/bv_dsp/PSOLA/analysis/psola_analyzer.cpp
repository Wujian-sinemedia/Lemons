
#include "GrainExtractor/GrainExtractor.cpp"


namespace bav::dsp
{
template<typename SampleType>
PsolaAnalyzer<SampleType>::PsolaAnalyzer()
{
    
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::analyzeInput (const juce::AudioBuffer<SampleType>& audio)
{
    analyzeInput (audio.getReadPointer(0), audio.getNumSamples());
}

template<typename SampleType>
void PsolaAnalyzer<SampleType>::analyzeInput (const SampleType* samples, int numSamples)
{
    jassert (samplerate > 0);
    
    const auto pitchInHz = pitchDetector.detectPitch (samples, numSamples);
    const auto frameIsPitched = pitchInHz > 0.f;
    
    const auto period = frameIsPitched ? math::periodInSamples (samplerate, pitchInHz)
                                       : getNextUnpitchedPeriod();
    
    grainExtractor.getGrainOnsetIndices (grainOnsetIndices, samples, numSamples, period);
    
    currentPeriod = period;
}

template<typename SampleType>
int PsolaAnalyzer<SampleType>::getNextUnpitchedPeriod()
{
    return rand.nextInt (pitchDetector.getCurrentLegalPeriodRange());
}

template<typename SampleType>
int PsolaAnalyzer<SampleType>::getClosestGrainOnset (int sampleIndex) const
{
    jassert (! grainOnsetIndices.isEmpty());
    
    auto leastDist = std::numeric_limits<int>::max();
    auto grainOnset = grainOnsetIndices.getUnchecked (0);
    
    for (auto index : grainOnsetIndices)
    {
        const auto distance = std::abs (index - sampleIndex);
        
        if (distance == 0)
            return index;
        
        if (distance < leastDist)
        {
            leastDist = distance;
            grainOnset = index;
        }
    }
    
    return grainOnset;
}


template class PsolaAnalyzer< float >;
template class PsolaAnalyzer< double >;

}  // namespace bav::dsp
