#pragma once

namespace lemons::dsp
{

template <typename SampleType>
class AudioFilePlayer : public LatencyEngine<SampleType>
{
public:
	AudioFilePlayer (AudioFile& file);

private:
	using InterpolatorType = juce::Interpolators::Lagrange;

	void renderChunk (const AudioBuffer<SampleType>&, AudioBuffer<SampleType>& output, MidiBuffer&, bool isBypassed) final;

	void onPrepare (int blocksize, double samplerate, int numChannels) final;

	void onRelease() final;

	const AudioBuffer<SampleType>& origAudio;

	const double origSamplerate;
	const int    origNumSamples;

	double speedRatio { 0. };

	juce::Array<int> readPositions;

	ConstructedArray<InterpolatorType> interpolators;
};

}  // namespace lemons::dsp
