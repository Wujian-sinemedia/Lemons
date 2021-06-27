
#pragma once

namespace bav::dsp::psola
{
/*
 This grain class plays an unadjusted stream of contiguous audio samples -- once it is started, it simply plays until it stops.
 */

template < typename SampleType >
class SynthesisGrain
{
public:
    SynthesisGrain (const CircularBuffer< SampleType >& storageToUse);

    bool isActive() const;
    bool isHalfwayThrough() const;

    void startNewGrain (int start, int length);

    SampleType getNextSample();

private:
    SampleType getWindowValue (int index);

    const CircularBuffer< SampleType >& buffer;

    bool active {false};

    int startIndex {0};
    int grainLength {0};

    int currentTick {0};
};

}  // namespace bav::dsp::psola
