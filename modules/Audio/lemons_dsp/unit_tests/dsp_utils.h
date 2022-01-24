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

#include <juce_audio_basics/juce_audio_basics.h>


namespace lemons::tests
{

template <typename FloatType>
using AudioBuffer = juce::AudioBuffer<FloatType>;


/** @ingroup lemons_dsp_unit_tests
 */
template <typename SampleType>
void fillAudioBufferWithRandomNoise (AudioBuffer<SampleType>& buffer, juce::Random rng);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
									const AudioBuffer<FloatType>& buffer2);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool buffersAreReasonablyEqual (const AudioBuffer<FloatType>& buffer1,
											  const AudioBuffer<FloatType>& buffer2);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool allSamplesAreEqual (const AudioBuffer<FloatType>& buffer1,
									   const AudioBuffer<FloatType>& buffer2,
									   int startIndex1, int numSamples,
									   int startIndex2 = -1,
									   int channel1	   = 0,
									   int channel2	   = -1);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool bufferChannelsAreEqual (const AudioBuffer<FloatType>& buffer1,
										   int							 channel1,
										   const AudioBuffer<FloatType>& buffer2,
										   int							 channel2);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
									  int startIndex, int numSamples, int channel = 0);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool bufferIsSilent (const AudioBuffer<FloatType>& buffer);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool allSamplesAreValid (const AudioBuffer<FloatType>& buffer);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename FloatType>
[[nodiscard]] bool noSamplesAreClipping (const AudioBuffer<FloatType>& buffer);

/** @ingroup lemons_dsp_unit_tests
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> makeCopyOfAudioBuffer (const AudioBuffer<SampleType>& inputBuffer);

}  // namespace lemons::tests
