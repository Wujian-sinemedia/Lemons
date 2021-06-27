
#pragma once

namespace bav::dsp::psola
{
template < typename SampleType >
class SynthesisGrain
{
public:
    SynthesisGrain (const AnalysisGrainStorage< SampleType >& storageToUse);

    bool isActive() const;

    void startNewGrain (int start, int length);

    SampleType getNextSample();

private:
    SampleType getWindowValue (int index) const noexcept;

    const AnalysisGrainStorage< SampleType >& storage;

    bool active {false};

    int startIndex {0};
    int grainLength {0};

    int currentTick {0};
};

}  // namespace bav::dsp::psola
