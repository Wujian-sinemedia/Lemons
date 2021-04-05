
/*
    ASDF-based pitch detector
*/

namespace bav::dsp
{
    
    
#define bvpd_NUM_PERIOD_CANDIDATES_TO_TEST 10
    

template<typename SampleType>
class PitchDetector
{
    
    using AudioBuffer = juce::AudioBuffer<SampleType>;
    
    
public:
    
    PitchDetector():
        minHz(0), maxHz(0),
        lastEstimatedPeriod(0), lastFrameWasPitched(false),
        samplerate(0.0), confidenceThresh(SampleType(0.15)),
        asdfBuffer(0, 0)
    { }
    
    ~PitchDetector() { }
    
    void initialize()
    {
        periodCandidates.ensureStorageAllocated (bvpd_NUM_PERIOD_CANDIDATES_TO_TEST);
        candidateDeltas.ensureStorageAllocated (bvpd_NUM_PERIOD_CANDIDATES_TO_TEST);
        weightedCandidateConfidence.ensureStorageAllocated(bvpd_NUM_PERIOD_CANDIDATES_TO_TEST);
        
        asdfBuffer.setSize (1, 512);
        
        lastFrameWasPitched = false;
    }
    
    void releaseResources()
    {
        periodCandidates.clear();
        candidateDeltas.clear();
        weightedCandidateConfidence.clear();
        
        asdfBuffer.setSize (0, 0, false, false, false);
    }
    
    float detectPitch (const AudioBuffer& inputAudio)
    {
        // this function returns the pitch in Hz, or 0.0f if the frame of audio is determined to be unpitched
        
        jassert (samplerate > 0);
        
        const auto numSamples = inputAudio.getNumSamples();
        const auto halfNumSamples = juce::roundToInt (floor (numSamples * 0.5f));
        
        jassert (numSamples >= 2 * maxPeriod);
        
        const auto* reading = inputAudio.getReadPointer(0);
        
        // the minPeriod & maxPeriod members define the overall global period range; here, the minLag & maxLag local variables are used to define the period range for this specific frame of audio, if it can be constrained more than the global range based on instantaneous conditions:
        
        auto minLag = samplesToFirstZeroCrossing (reading, numSamples);  // period cannot be smaller than the # of samples to the first zero crossing
        auto maxLag = halfNumSamples;
        
        if (lastFrameWasPitched)  // pitch shouldn't halve or double between consecutive voiced frames
        {
            minLag = std::max (minLag, juce::roundToInt (lastEstimatedPeriod * SampleType(0.5)));
            maxLag = std::min (maxLag, juce::roundToInt (lastEstimatedPeriod * SampleType(2)));
        }
        
        minLag = std::max (minLag, minPeriod);
        maxLag = std::min (maxLag, maxPeriod);
        
        if (! (maxLag > minLag)) // truncation of edge cases
            minLag = std::min (maxLag - 1, minPeriod);
        
        jassert (maxLag > minLag);
        
        auto* asdfData = asdfBuffer.getWritePointer(0);
        
        vecops::fill (asdfData, SampleType(0), asdfBuffer.getNumSamples());
        
        // COMPUTE ASDF
        
        jassert (asdfBuffer.getNumSamples() >= maxLag - minLag + 1);
        
        for (int k = minLag; k <= maxLag; ++k)  // k = lag = period
        {
            const auto index = k - minLag; // the actual asdfBuffer index for this k value's data. offset = minPeriod
            
            for (int s1 = 0, s2 = halfNumSamples;
                 s1 < halfNumSamples && s2 < numSamples;
                 ++s1, ++s2)
            {
                const auto difference = ( (reading[s1] - reading[s1 + k])
                                         + (reading[s2 - k] - reading[s2]) );
                
                asdfData[index] += (difference * difference);
            }
        }
        
        const auto asdfDataSize = maxLag - minLag + 1;
        
        vecops::normalize (asdfData, asdfDataSize);
        
        int minIndex = 0;
        SampleType greatestConfidence = 0;
        
        vecops::findMinAndMinIndex (asdfData, asdfDataSize, greatestConfidence, minIndex);
        
        jassert (minIndex >= 0 && minIndex <= asdfDataSize);
        
        if (greatestConfidence > confidenceThresh) // determine if frame is unpitched - not enough periodicity
        {
            lastFrameWasPitched = false;
            return 0.0f;
        }
        
        if (lastFrameWasPitched)
            minIndex = chooseIdealPeriodCandidate (asdfData, asdfDataSize, minIndex);
        
        const auto realPeriod = minIndex + minLag;   // account for offset in asdf data (index 0 stored data for lag of minPeriod)
        
        jassert (realPeriod <= maxPeriod && realPeriod >= minPeriod);
        
        lastEstimatedPeriod = realPeriod;
        lastFrameWasPitched = true;
        
        return float(samplerate / realPeriod); // return pitch in hz
    }
    
    
    int getLatencySamples() const noexcept { return 2 * maxPeriod; }
    
    
    void setHzRange (const int newMinHz, const int newMaxHz)
    {
        jassert (newMaxHz > newMinHz);
        jassert (newMinHz > 0 && newMaxHz > 0);
        
        minHz = newMinHz;
        maxHz = newMaxHz;
        
        if (samplerate == 0)
            return;
        
        maxPeriod = juce::roundToInt (samplerate / minHz);
        minPeriod = juce::roundToInt (samplerate / maxHz);
        
        if (minPeriod < 1)
            minPeriod = 1;
        
        if (! (maxPeriod > minPeriod))
            maxPeriod = minPeriod + 1;
        
        const auto numOfLagValues = maxPeriod - minPeriod + 1;
        
        if (asdfBuffer.getNumSamples() != numOfLagValues)
            asdfBuffer.setSize (1, numOfLagValues, true, true, true);
    }
    
    
    void setConfidenceThresh (const SampleType newThresh) { confidenceThresh = newThresh; }
    
    
    void setSamplerate (const double newSamplerate)
    {
        jassert (newSamplerate > 0);
        
        if (lastFrameWasPitched)
            lastEstimatedPeriod = juce::roundToInt (newSamplerate / (samplerate / lastEstimatedPeriod));
        
        samplerate = newSamplerate;
        
        if (minHz > 0 && maxHz > 0)
            setHzRange (minHz, maxHz);
    }
    
    /*
    */
    
private:
    
    int chooseIdealPeriodCandidate (const SampleType* asdfData, const int asdfDataSize, const int minIndex)
    {
        periodCandidates.clearQuick();
        candidateDeltas.clearQuick();
        weightedCandidateConfidence.clearQuick();
        
        periodCandidates.add (minIndex);
        
        for (int c = 1; c <= bvpd_NUM_PERIOD_CANDIDATES_TO_TEST; ++c)
            getNextBestPeriodCandidate (periodCandidates, asdfData, asdfDataSize);
        
        jassert (! periodCandidates.isEmpty());
        
        if (periodCandidates.size() <= 2)
            return minIndex;
        
        const auto adding = minPeriod - lastEstimatedPeriod;
        
        // candidate deltas: how far away each period candidate is from the last estimated period
        for (int candidate : periodCandidates)
            candidateDeltas.add (abs (candidate + adding));
        
        // find the difference between the max & min delta values of the candidates
        const auto deltaRange = intops::findRangeOfExtrema (candidateDeltas.getRawDataPointer(), candidateDeltas.size());
        
        if (deltaRange < 2)  // prevent dividing by zero in the next step...
            return minIndex;
        
        // weight the asdf data based on each candidate's delta value
        // because higher asdf values represent a lower confidence in that period candidate, we want to artificially increase the asdf data a bit for candidates with higher deltas
        
        for (int c = 0; c < periodCandidates.size(); ++c)
            weightedCandidateConfidence.add (asdfData[periodCandidates.getUnchecked(c)] * (candidateDeltas.getUnchecked(c) / deltaRange));
        
        // choose the estimated period based son the lowest weighted asdf data value:
        return periodCandidates.getUnchecked (vecops::findIndexOfMinElement (weightedCandidateConfidence.getRawDataPointer(),
                                                                             weightedCandidateConfidence.size()));
    }
    
    
    void getNextBestPeriodCandidate (juce::Array<int>& candidates, const SampleType* asdfData, const int dataSize)
    {
        int index = -1;
        
        for (int i = 0; i < dataSize; ++i)
        {
            if (! candidates.contains (i))
            {
                index = i;
                break;
            }
        }
        
        if (index == -1)
            return;
        
        auto min = asdfData[index];
        
        for (int i = 0; i < dataSize; ++i)
        {
            if (candidates.contains (i))
                continue;
            
            const auto current = asdfData[i];
            
            if (current < min)
            {
                min = current;
                index = i;
            }
        }
        
        candidates.add (index);
    }
    
    
    int samplesToFirstZeroCrossing (const SampleType* inputAudio, const int numSamples)
    {
#if BV_USE_VDSP
        unsigned long index = 0; // in Apple's vDSP, the type vDSP_Length is an alias for unsigned long
        unsigned long totalcrossings = 0;
        
        if constexpr (std::is_same_v <SampleType, float>)
            vDSP_nzcros (inputAudio, vDSP_Stride(1), vDSP_Length(1), &index, &totalcrossings, vDSP_Length(numSamples));
        else
            vDSP_nzcrosD (inputAudio, vDSP_Stride(1), vDSP_Length(1), &index, &totalcrossings, vDSP_Length(numSamples));
        
        return int(index);
#else
        const bool startedPositive = inputAudio[0] > SampleType(0);
        
        for (int s = 1; s < numInputSamples; ++s)
        {
            const auto currentSample = inputAudio[s];
            
            if (startedPositive != (currentSample > SampleType(0)))
                return s;
        }
        
        return 0;
#endif
    }
    
    /*
    */
    
    int minHz, maxHz;
    int minPeriod, maxPeriod;
    
    int lastEstimatedPeriod;
    bool lastFrameWasPitched;
    
    double samplerate;
    
    SampleType confidenceThresh;  // if the lowest asdf data value is above this thresh, the frame of audio is determined to be unpitched
    
    AudioBuffer asdfBuffer; // calculated ASDF values will be placed in this buffer
    
    juce::Array<int> periodCandidates;
    juce::Array<int> candidateDeltas;
    juce::Array<SampleType> weightedCandidateConfidence;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

    
template class PitchDetector<float>;
template class PitchDetector<double>;
    
    
#undef bvpd_NUM_PERIOD_CANDIDATES_TO_TEST
    
} // namespace



