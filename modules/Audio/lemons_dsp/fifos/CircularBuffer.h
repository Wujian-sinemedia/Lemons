#pragma once

namespace lemons::dsp
{

template <typename SampleType>
class CircularBuffer
{
public:

	void storeSamples (const AudioBuffer<SampleType>& samples, int channel = 0);
	void storeSamples (const SampleType* samples, int numSamples);

	void getSamples (AudioBuffer<SampleType>& output, int channel = 0);
	void getSamples (SampleType* output, int numSamples);

	int getCapacity() const noexcept;
	int getNumStoredSamples() const noexcept;

	void resize (int newSize);

	void clear();

private:
	AudioBuffer<SampleType> buffer;

	int lastFrameStart { 0 };
	int lastFrameEnd { 0 };

	int numStoredSamples { 0 };
};

}  // namespace lemons::dsp
