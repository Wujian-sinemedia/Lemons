#pragma once

namespace lemons::dsp
{

template <typename SampleType>
class AudioFifo
{
public:

	void pushSamples (const AudioBuffer<SampleType>& input);

	void popSamples (AudioBuffer<SampleType>& output);

	int numStoredSamples() const noexcept;

	void clear();

	void resize (int maxNumSamples, int numChannels = 2);

private:
	juce::OwnedArray<CircularBuffer<SampleType>> buffers;
};

}  // namespace lemons::dsp
