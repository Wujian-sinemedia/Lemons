
#pragma once

namespace bav
{

/*------------------------------------------------------------------------------------------------------------------------------------------------------
 SynthesisGrain :   This class holds a pointer to a specific AnalysisGrain, and its respacing information so it can be used to create a stream of repitched audio.
 ------------------------------------------------------------------------------------------------------------------------------------------------------*/

template < typename SampleType >
class SynthesisGrain
{
    using Grain = AnalysisGrain< SampleType >;
    
public:
    SynthesisGrain();
    
    Grain* orig() const noexcept;
    
    bool isActive() const noexcept;
    
    bool isHalfwayThrough() const noexcept;
    
    void startNewGrain (Grain* newGrain, int samplesInFuture);
    
    SampleType getNextSample();
    
    int samplesLeft() const noexcept;
    
    int silenceLeft() const noexcept;
    
    int getSize() const noexcept;
    
    void stop() noexcept;
    
    
private:
    int    readingIndex; // the next index to be read from the AnalysisGrain's buffer
    Grain* grain;
    int zeroesLeft; // the number of zeroes this grain will output before actually outputting its samples. This allows grains to be respaced into the future.
    int halfIndex; // marks the halfway point for this grain
};

template class SynthesisGrain< float >;
template class SynthesisGrain< double >;

}
