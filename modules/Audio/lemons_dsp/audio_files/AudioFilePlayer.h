/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#pragma once

namespace lemons::dsp
{

/** @ingroup audio_files audio_engines
    A kind of AudioEngine that plays audio from an AudioFile object.
    @see AudioFile
 */
template <typename SampleType>
class AudioFilePlayer final : public LatencyEngine<SampleType>
{
public:
	/** Constructor. */
	explicit AudioFilePlayer (AudioFile& file);

private:
	using InterpolatorType = juce::Interpolators::Lagrange;

	void renderChunk (const AudioBuffer<SampleType>&, AudioBuffer<SampleType>& output, MidiBuffer&, bool isBypassed) final;

	void prepared (int, double samplerate, int) final;

	void onRelease() final;

	const AudioBuffer<float>& origAudio;

	const double origSamplerate;
	const int    origNumSamples;

	double speedRatio { 0. };

	juce::Array<int> readPositions;

	ConstructedArray<InterpolatorType> interpolators;

	AudioBuffer<float> conversionBuffer;  // needed for double version only
};

}  // namespace lemons::dsp
