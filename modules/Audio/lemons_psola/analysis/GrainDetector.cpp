namespace lemons::dsp::psola
{

template<typename SampleType>
void GrainDetector<SampleType>::prepare (int maxBlocksize)
{
    jassert (maxBlocksize > 0);
    
    grainStartIndices.ensureStorageAllocated (maxBlocksize);
    peakSearchingOrder.ensureStorageAllocated (maxBlocksize);
    peakCandidates.ensureStorageAllocated (numPeaksToTest + 1);
    candidateDeltas.ensureStorageAllocated (numPeaksToTest + 1);
    finalHandful.ensureStorageAllocated (defaultFinalHandfulSize);
    finalHandfulDeltas.ensureStorageAllocated (defaultFinalHandfulSize);
    
    grainStartIndices.clearQuick();
    peakSearchingOrder.clearQuick();
    peakCandidates.clearQuick();
    candidateDeltas.clearQuick();
    finalHandful.clearQuick();
    finalHandfulDeltas.clearQuick();
}

template<typename SampleType>
void GrainDetector<SampleType>::releaseResources()
{
    grainStartIndices.clear();
    peakSearchingOrder.clear();
    peakCandidates.clear();
    finalHandful.clear();
    candidateDeltas.clear();
    finalHandfulDeltas.clear();
}

template<typename SampleType>
const juce::Array<int>& GrainDetector<SampleType>::analyzeInput (const SampleType* inputSamples,
                                                                 int numSamples,
                                                                 float period)
{
    jassert (period > 0.f);
    jassert (numSamples >= juce::roundToInt (period * 2.f));
    
    findPeaks (inputSamples, numSamples, period);
    
    jassert (! grainStartIndices.isEmpty());
    
//    const auto grainLength = period * 2;
//    const auto halfPeriod  = juce::roundToInt (period * 0.5f);
//
//    // create array of grain start indices, such that grains are 2 pitch periods long, CENTERED on points of synchronicity previously identified
//
//    for (int i = 0; i < grainStartIndices.size(); ++i)
//    {
//        // offset the peak index by the period so that the peak index will be in the center of the grain (if grain is 2 periods long)
//        auto grainStart = grainStartIndices.getUnchecked (i) - juce::roundToInt (period);
//
//        if (grainStart < 0)
//        {
//            if (i < grainStartIndices.size() - 2 || grainStartIndices.size() > 1) continue;
//
//            while (grainStart < 0)
//                grainStart += halfPeriod;
//        }
//
//        if (grainStart + grainLength > numSamples)
//        {
//            if (i < grainStartIndices.size() - 2 || grainStartIndices.size() > 1) continue;
//
//            while (grainStart + grainLength > numSamples)
//                grainStart -= halfPeriod;
//
//            if (grainStart < 0)
//            {
//                if (! grainStartIndices.isEmpty()) continue;
//                grainStart = 0;
//            }
//        }
//
//        if (grainStart < 0 || grainStart + grainLength > numSamples)
//            continue;
//
//        grainStartIndices.add (grainStart);
//    }
//
//    jassert (! grainStartIndices.isEmpty());
    
    return grainStartIndices;
}

template<typename SampleType>
void GrainDetector<SampleType>::findPeaks (const SampleType* inputSamples, int numSamples, float period)
{
    grainStartIndices.clearQuick();
    
    // output grains are 2 periods long w/ 50% overlap
    const auto grainSize  = juce::roundToInt (2.f * period);
    const auto halfPeriod = juce::roundToInt (period * 0.5f);
    const auto intPeriod  = juce::roundToInt (period);
    
    jassert (numSamples >= grainSize);
    
    // marks the center of the analysis windows, which are 1 period long
    auto analysisIndex = halfPeriod;
    
    do
    {
        const auto frameStart = analysisIndex - halfPeriod;
        const auto frameEnd   = std::min (numSamples, frameStart + juce::roundToInt (period));
        
        jassert (frameStart >= 0 && frameEnd <= numSamples);
        
        grainStartIndices.add (findNextPeak (frameStart, frameEnd, std::min (analysisIndex, frameEnd),
                                             inputSamples, intPeriod, grainSize));
        
        jassert (! grainStartIndices.isEmpty());
        
        const auto prevAnalysisIndex = analysisIndex;
        const auto targetArraySize   = grainStartIndices.size();
        
        // analysisIndex marks the middle of our next analysis window, so it's where our next predicted peak should be:
        analysisIndex = targetArraySize == 1 ? grainStartIndices.getUnchecked (0) + intPeriod
                                             : grainStartIndices.getUnchecked (targetArraySize - 2) + grainSize;
        
        if (analysisIndex == prevAnalysisIndex)
            analysisIndex = prevAnalysisIndex + intPeriod;
        else
            jassert (analysisIndex > prevAnalysisIndex);
    }
    while (analysisIndex - halfPeriod < numSamples);
}

template<typename SampleType>
int GrainDetector<SampleType>::findNextPeak (int frameStart, int frameEnd, int predictedPeak,
                                             const SampleType* inputSamples, float period, int grainSize)
{
    jassert (frameEnd > frameStart);
    jassert (predictedPeak >= frameStart && predictedPeak <= frameEnd);
    
    sortSampleIndicesForPeakSearching (frameStart, frameEnd, predictedPeak);
    
    jassert (peakSearchingOrder.size() == frameEnd - frameStart);
    
    peakCandidates.clearQuick();
    
    for (int i = 0; i < numPeaksToTest; ++i)
        getPeakCandidateInRange (inputSamples, frameStart, frameEnd, predictedPeak);
    
    jassert (! peakCandidates.isEmpty());
    
    switch (peakCandidates.size())
    {
        case 1 : return peakCandidates.getUnchecked (0);
            
        case 2 : return choosePeakWithGreatestPower (inputSamples);
            
        default :
        {
            if (grainStartIndices.size() <= 1)
                return choosePeakWithGreatestPower (inputSamples);
            
            return chooseIdealPeakCandidate (inputSamples,
                                             grainStartIndices.getLast() + juce::roundToInt (period),
                                             grainStartIndices.getUnchecked (grainStartIndices.size() - 2) + grainSize);
        }
    }
}

static inline double weighting_func (int index, int predicted, int numSamples)
{
    return 1. - ((static_cast<double>(std::abs (index - predicted)) / static_cast<double>(numSamples)) * 0.5);
}

template<typename SampleType>
void GrainDetector<SampleType>::getPeakCandidateInRange (const SampleType* inputSamples, int startSample, int endSample, int predictedPeak)
{
    jassert (! peakSearchingOrder.isEmpty());
    
    int starting = -1;
    
    for (int poss : peakSearchingOrder)
    {
        if (! peakCandidates.contains (poss))
        {
            starting = poss;
            break;
        }
    }
    
    if (starting == -1) return;
    
    jassert (starting >= startSample && starting <= endSample);
    
    const auto numSamples = endSample - startSample;
    
    auto localMin        = inputSamples[starting] * static_cast< SampleType > (weighting_func (starting, predictedPeak, numSamples));
    auto localMax        = localMin;
    auto indexOfLocalMin = starting;
    auto indexOfLocalMax = starting;
    
    for (int index : peakSearchingOrder)
    {
        if (index == starting || peakCandidates.contains (index)) continue;
        
        jassert (index >= startSample && index <= endSample);
        
        const auto currentSample =
        inputSamples[index] * static_cast< SampleType > (weighting_func (index, predictedPeak, numSamples));
        
        if (currentSample < localMin)
        {
            localMin        = currentSample;
            indexOfLocalMin = index;
        }
        
        if (currentSample > localMax)
        {
            localMax        = currentSample;
            indexOfLocalMax = index;
        }
    }
    
    if (indexOfLocalMax == indexOfLocalMin)
    {
        peakCandidates.add (indexOfLocalMax);
    }
    else if (localMax < SampleType (0.0))
    {
        peakCandidates.add (indexOfLocalMin);
    }
    else if (localMin > SampleType (0.0))
    {
        peakCandidates.add (indexOfLocalMax);
    }
    else
    {
        peakCandidates.add (std::min (indexOfLocalMax, indexOfLocalMin));
        peakCandidates.add (std::max (indexOfLocalMax, indexOfLocalMin));
    }
}

template<typename SampleType>
int GrainDetector<SampleType>::choosePeakWithGreatestPower (const SampleType* inputSamples) const
{
    auto strongestPeakIndex = peakCandidates.getUnchecked (0);
    auto strongestPeak      = std::abs (inputSamples[strongestPeakIndex]);
    
    for (int candidate : peakCandidates)
    {
        const auto current = std::abs (inputSamples[candidate]);
        
        if (current > strongestPeak)
        {
            strongestPeak      = current;
            strongestPeakIndex = candidate;
        }
    }
    
    return strongestPeakIndex;
}

static inline float delta_weight (float delta, float totalDeltaRange)
{
    return 1.f - (delta / totalDeltaRange);
}

template<typename SampleType>
int GrainDetector<SampleType>::chooseIdealPeakCandidate (const SampleType* inputSamples, int deltaTarget1, int deltaTarget2)
{
    candidateDeltas.clearQuick();
    finalHandful.clearQuick();
    finalHandfulDeltas.clearQuick();
    
    // 1. calculate delta values for each peak candidate
    // delta represents how far off this peak candidate is from the expected peak location - in a way it's a measure of the jitter that picking a peak candidate as this frame's peak would introduce to the overall alignment of the stream of grains based on the previous grains
    
    for (int candidate : peakCandidates)
    {
        candidateDeltas.add ((std::abs (candidate - deltaTarget1) + abs (candidate - deltaTarget2))
                             * 0.5f);
    }
    
    // 2. whittle our remaining candidates down to the final candidates with the minimum delta values
    
    const auto finalHandfulSize =
    std::min (defaultFinalHandfulSize, candidateDeltas.size());
    
    float      minimum      = 0.0f;
    int        minimumIndex = 0;
    const auto dataSize     = candidateDeltas.size();
    
    for (int i = 0; i < finalHandfulSize; ++i)
    {
        vecops::findMinAndMinIndex (candidateDeltas.getRawDataPointer(), dataSize, minimum, minimumIndex);
        
        finalHandfulDeltas.add (minimum);
        finalHandful.add (peakCandidates.getUnchecked (minimumIndex));
        
        // make sure this value won't be chosen again, w/o deleting it from the candidateDeltas array
        candidateDeltas.set (minimumIndex,  10000.0f);
    }
    
    jassert (finalHandful.size() == finalHandfulSize
             && finalHandfulDeltas.size() == finalHandfulSize);
    
    // 3. choose the strongest overall peak from these final candidates, with peaks weighted by their delta values
    
    const auto deltaRange = vecops::findRangeOfExtrema (finalHandfulDeltas.getRawDataPointer(), finalHandfulDeltas.size());
    
    if (deltaRange < 0.05f)  // prevent dividing by 0 in the next step...
        return finalHandful.getUnchecked (0);
    
    auto chosenPeak = finalHandful.getUnchecked (0);
    auto strongestPeak =
    std::abs (inputSamples[chosenPeak])
    * delta_weight (finalHandfulDeltas.getUnchecked (0), deltaRange);
    
    for (int i = 1; i < finalHandfulSize; ++i)
    {
        const auto candidate = finalHandful.getUnchecked (i);
        
        if (candidate == chosenPeak) continue;
        
        const auto testingPeak =
        std::abs (inputSamples[candidate])
        * delta_weight (finalHandfulDeltas.getUnchecked (i), deltaRange);
        
        if (testingPeak > strongestPeak)
        {
            strongestPeak = testingPeak;
            chosenPeak    = candidate;
        }
    }
    
    return chosenPeak;
}

template<typename SampleType>
void GrainDetector<SampleType>::sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak)
{
    peakSearchingOrder.clearQuick();
    
    jassert (predictedPeak >= startSample && predictedPeak <= endSample);
    
    peakSearchingOrder.set (0, predictedPeak);
    
    for (int p = 1, m = -1, n = 1;
         n < (endSample - startSample);
         ++n)
    {
        const auto pos = predictedPeak + p;
        const auto neg = predictedPeak + m;
        
        if (math::numberIsEven (n))
        {
            if (neg >= startSample)
            {
                peakSearchingOrder.set (n, neg);
                --m;
            }
            else
            {
                jassert (pos <= endSample);
                peakSearchingOrder.set (n, pos);
                ++p;
            }
        }
        else
        {
            if (pos <= endSample)
            {
                peakSearchingOrder.set (n, pos);
                ++p;
            }
            else
            {
                jassert (neg >= startSample);
                peakSearchingOrder.set (n, neg);
                --m;
            }
        }
    }
}

template class GrainDetector<float>;
template class GrainDetector<double>;

}


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template<typename SampleType>
GrainDetectorTests<SampleType>::GrainDetectorTests()
: DspTest ("PSOLA grain detector tests")
{ }

template<typename SampleType>
void GrainDetectorTests<SampleType>::runTest()
{
    /*
     Heuristics:
     - grains are approx 2 periods long
     - grains overlap by approx 50%
     - grains are centered on points of max energy in signal
     - individual grain onsets are approx 1 period apart
     */
    
    constexpr auto samplerate = 44100.;
    
    for (const auto freq : { 215., 440., 755., 2036. })
    {
        const auto period = math::periodInSamples (samplerate, freq);
        
        beginTest ("Period: " + String(period) + " samples");
        
        const auto blocksize = period * 4;
        
        grainDetector.prepare (blocksize);
        
        audioStorage.setSize (1, blocksize);
        
        osc.setFrequency (static_cast<SampleType> (freq),
                          static_cast<SampleType> (samplerate));
        
        osc.getSamples (audioStorage);
        
        const auto& indices = grainDetector.analyzeInput (audioStorage.getReadPointer(0), blocksize, period);
        
        {
            const auto subtest = beginSubtest ("Grains identified");
            expectGreaterOrEqual (indices.size(), blocksize / period);
        }
        
        {
            const auto subtest = beginSubtest ("Indices are always increasing");
            
            for (int i = 0; i < indices.size() - 1; ++i)
                expectGreaterThan (indices.getUnchecked (i + 1),
                                   indices.getUnchecked (i));
        }
        
        {
            const auto subtest = beginSubtest ("Max and min index are within range");
            
            expectGreaterOrEqual (indices.getUnchecked (0), 0);
            
            expectLessOrEqual (indices.getUnchecked (indices.size() - 1), blocksize);
        }
        
        const auto subtest = beginSubtest ("Grain spacing");
        
        const auto quarterPeriod = period / 4;
        
        for (int i = 0; i < indices.size() - 2; ++i)
        {
            const auto index1 = indices.getUnchecked (i);
            const auto index2 = indices.getUnchecked (i + 1);
            const auto index3 = indices.getUnchecked (i + 2);
            
            expectWithinAbsoluteError (index3 - index1, period * 2, quarterPeriod);
            
            expectWithinAbsoluteError (index2 - index1, period, quarterPeriod);
            expectWithinAbsoluteError (index3 - index2, period, quarterPeriod);
        }
    }
}

template struct GrainDetectorTests<float>;
template struct GrainDetectorTests<double>;

}

#endif
