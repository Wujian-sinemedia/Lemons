
#pragma once

namespace lemons::dsp::psola
{
/** Stores the analysis grains identified by the Analyzer so they can be resynthesized by Shifter objects.
    This class isn't really meant for public use; it's more of a support class for Analyzer and Shifter.
    @see Analyzer, Shifter
 */
template < typename SampleType >
class AnalysisGrainStorage
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    /** Prepares the storage for a new maximum blocksize. */
    void prepare (int blocksize);

    /** Stores a new frame of analyzed samples. */
    void storeNewFrame (const SampleType*         inputSamples,
                        int                       numSamples,
                        const juce::Array< int >& grainOnsets);

    /** Returns the start of the closest grain to the requested sample index for this frame. */
    int getStartOfClosestGrain (int sampleIndex) const;

    /** Returns a sample value. */
    SampleType getSample (int grainStartIndexInCircularBuffer, int grainTick) const;

private:
    int blockIndexToBufferIndex (int blockIndex) const;

    CircularBuffer< SampleType > buffer;

    juce::Array< int >         grainIndices;  // sample indices in the circular buffer of grain onsets
    mutable juce::Array< int > distances;     // array for sorting distances of grain onsets
};

}  // namespace lemons::dsp::psola
