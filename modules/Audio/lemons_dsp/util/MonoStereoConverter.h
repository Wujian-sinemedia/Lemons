
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons::dsp
{

/** @defgroup lemons_stereo_conversion Mono/stereo conversion
	@ingroup lemons_dsp
	Mono/stereo conversion utilities.
 */

/** @ingroup lemons_stereo_conversion
	An enum used to describe how a MonoStereoConverter will convert stereo signals to mono. */
enum class StereoReductionMode
{
	/** In this mode, a MonoStereoConverter will output only the left channel of a stereo signal as its mono reduction. */
	leftOnly,

	/** In this mode, a MonoStereoConverter will output only the right channel of a stereo signal as its mono reduction. */
	rightOnly,

	/** In this mode, a MonoStereoConverter will mix together the two channels of a stereo signal to create its mono reduction. */
	mixToMono
};

/** @ingroup lemons_stereo_conversion
	A utility class for converting mono signals to stereo, or stereo signals to mono.
 */
template <typename SampleType>
class MonoStereoConverter final
{
public:

	/** Default constructor. */
	MonoStereoConverter() = default;

	/** Prepares the converter to process a new maximum blocksize. */
	void prepare (int blocksize);

	/** Sets the StereoReductionMode the converter will use for converting stereo signals to mono. */
	void setStereoReductionMode (StereoReductionMode newmode);

	/** Returns the StereoReductionMode the converter is currently using. */
	[[nodiscard]] StereoReductionMode getStereoReductionMode() const noexcept { return toMonoMode; }

	/** Converts a stereo signal to mono. */
	void convertStereoToMono (const SampleType* const leftIn,
							  const SampleType* const rightIn,
							  SampleType* const		  monoOut,
							  int					  numSamples);

	/** Converts a stereo signal to mono. */
	void convertStereoToMono (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output);

	/** Converts a mono signal to stereo. */
	void convertMonoToStereo (const SampleType* const monoIn,
							  SampleType* const		  leftOut,
							  SampleType* const		  rightOut,
							  int					  numSamples);

	/** Converts a mono signal to stereo. */
	void convertMonoToStereo (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output);


private:

	StereoReductionMode toMonoMode { StereoReductionMode::leftOnly };

	AudioBuffer<SampleType> monoStorage;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonoStereoConverter)
};


}  // namespace lemons::dsp
