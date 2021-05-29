
#pragma once

namespace bav
{
/*------------------------------------------------------------------------------------------------------------------------------------------------------
 AnalysisGrain :    This class stores the actual audio samples that comprise a single audio grain, with a Hann window applied.
 ------------------------------------------------------------------------------------------------------------------------------------------------------*/

template < typename SampleType >
class AnalysisGrain
{
public:
    AnalysisGrain();

    void reserveSize (int numSamples);

    void incNumActive() noexcept;

    void decNumActive() noexcept;

    int numReferences() const noexcept;

    float percentOfExpectedSize() const noexcept;

    void storeNewGrain (const SampleType* inputSamples, int startSample, int endSample, int expectedGrainSize);

    void clear() noexcept;

    SampleType getSample (int index) const;

    int getSize() const noexcept;

    int pitchMark() const noexcept;

    bool isEmpty() const noexcept;


private:
    SampleType getWindowValue (int windowSize, int index) const;


    int numActive;  // this counts the number of SynthesisGrains that are referring to this AnalysisGrain

    int origStart;  // the original start & end sample indices of this grain

    int size;

    float percentOfExpectedGrainSize;

    juce::AudioBuffer< SampleType > samples;
};

}  // namespace bav
