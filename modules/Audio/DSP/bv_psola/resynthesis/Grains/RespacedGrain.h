#pragma once

#include "SynthesisGrain.h"

namespace bav::dsp::psola
{
template < typename SampleType >
class RespacedGrain
{
public:
    RespacedGrain (const CircularBuffer< SampleType >& storageToUse);

    bool isActive() const;
    bool isHalfwayThrough() const;

    SampleType getNextSample();

    void startNewGrain (int start, int length, int numZeroesFirst);

private:
    SynthesisGrain< SampleType > grain;

    int zeroesLeft {0};
};

}  // namespace bav::dsp::psola
