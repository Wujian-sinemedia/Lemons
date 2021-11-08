#pragma once

#include "Coeffecients.h"

namespace lemons::dsp::filters
{
/** A basic IIR filter with coefficients that can be reassigned during realtime playback.
    @see FX::Filter, MultiFilter
 */
template <typename SampleType>
class Filter
{
public:
	/** Resets the internal state of the filter. */
	void reset (SampleType resetToValue = SampleType (0));

	/** Prepares the filter to process audio. */
	void prepare() noexcept;

	/** Applies the filter to a stream of audio samples. */
	void process (SampleType* buffer, int numSamples);

	/** The filter's coefficients.
	    You can call various methods of this object to create different kinds of filters.
	 */
	Coefficients<SampleType> coefs;

private:
	void processOrder1 (SampleType* buffer, int numSamples);
	void processOrder2 (SampleType* buffer, int numSamples);
	void processOrder3 (SampleType* buffer, int numSamples);
	void processDefault (SampleType* buffer, int numSamples);

	void snapToZero() noexcept;

	juce::Array<SampleType> state;
	int               order = 0;
};

/*--------------------------------------------------------------------------------------------------------------*/

/** A class that holds an array of filters and keeps their coefficients in sync, for processing multichannel audio.
    @see Filter
 */
template <typename SampleType, size_t numChannels = 2>
struct MultiFilter
{
	using AudioBuffer = juce::AudioBuffer<SampleType>;

	/** Creates a default MultiFilter. */
	MultiFilter();

	/** Resets the internal state of the filters. */
	void reset (SampleType resetToValue = SampleType (0));

	/** Prepares the filters for processing. */
	void prepare() noexcept;

	/** Processes multichannel audio.
	    @see processChannel()
	 */
	void process (AudioBuffer& buffer) noexcept;

	/** Processes a single channel of audio in isolation.
	    @see process()
	 */
	void processChannel (int channel, SampleType* audio, int numSamples) noexcept;

	/** The raw coefficients. You can call methods of this object to create various filter topologies. */
	Coefficients<SampleType> coefs;

	/** The array of filters. */
	juce::OwnedArray<Filter<SampleType>> filters;
};

}  // namespace lemons::dsp::filters
