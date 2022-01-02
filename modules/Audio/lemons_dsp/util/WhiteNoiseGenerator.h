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

/** A simple processor that generates random noise.
 */
template <typename SampleType>
class WhiteNoiseGenerator final
{
public:
	/** Returns the next sample of random noise. */
	[[nodiscard]] SampleType getNextSample();

	/** Outputs a stream of random samples. */
	void getSamples (SampleType* output, int numSamples);

	/** Fills all channels of the AudioBuffer with random noise. */
	void getSamples (AudioBuffer<SampleType>& output);

private:
	juce::Random rand;
};

}  // namespace lemons::dsp
