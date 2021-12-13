#pragma once

namespace lemons::dsp
{

template <typename SampleType>
class WhiteNoiseGenerator final
{
public:
    explicit WhiteNoiseGenerator() = default;
    
	SampleType getNextSample();

	void getSamples (SampleType* output, int numSamples);

	void getSamples (AudioBuffer<SampleType>& output, int channel = 0);

private:
	juce::Random rand;
};

}  // namespace lemons::dsp
