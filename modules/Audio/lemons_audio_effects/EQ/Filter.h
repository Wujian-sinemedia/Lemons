
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
 *  ======================================================================================
 */

#pragma once

namespace lemons::dsp::FX
{
/**
	An enum class that represents the type of filter topology to be used by a Filter effect.
	@see Filter
 */
enum class FilterType
{
	LowPass,
	HighPass,
	LowShelf,
	HighShelf,
	BandPass,
	Notch,
	Peak,
	AllPass
};


/**
	A filter audio effect class that wraps the more low-level filters::Filter.
	@see FilterType, filters::Filter, filters::MultiFilter
 */
template <typename SampleType>
class Filter : public AudioEffect<SampleType>
{
public:

	/** Creates a default filter. */
	Filter() = default;

	/** Creates a filter with specified parameters. */
	explicit Filter (FilterType filterType, float frequency = 440.f, float Qfactor = 0.70710678118654752440f, float gainMult = 1.f);

	/** Sets the type of this filter. */
	void setFilterType (FilterType newType);

	/** Returns the current type of this filter. */
	FilterType getFilterType() const;

	/** Sets the frequency of this filter. */
	void setFilterFrequency (float newFreqHz);

	/** Returns the current frequency of this filter. */
	float getFilterFrequency() const;

	/** Sets the Q factor of this filter. */
	void setQfactor (float newQ);

	/** Returns the current Q factor of this filter. */
	float getQfactor() const;

	/** Sets the gain of this filter.
		Note that gain is only relevant for shelf and peak filter types.
	*/
	void setGain (float newGain);

	/** Returns the current gain of this filter.
		Note that gain is only relevant for shelf and peak filter types.
	*/
	float getGain() const;

	/** Applies the filter to a stream of audio. */
	void process (juce::AudioBuffer<SampleType>& audio) final;

	/** Prepares the filter. */
	void prepare (double samplerate, int blocksize) final;


private:

	filters::MultiFilter<SampleType, 2> filter;

	FilterType type { FilterType::HighPass };
	SampleType freq { static_cast<SampleType> (440.) };
	SampleType Q { static_cast<SampleType> (0.70710678118654752440L) };
	SampleType gain { static_cast<SampleType> (1.) };

	double sampleRate { 44100. };
};


/**
	A class that represents an audio engine holding a filter effect.
	@see FX::Filter
 */
template <typename SampleType>
struct FilterEngine final : EffectEngine<Filter, SampleType>
{
	using EffectEngine<Filter, SampleType>::EffectEngine;

	/** Returns a reference to the underlying filter effect object. */
	Filter<SampleType>& getFilter()
	{
		return (*this)();
	}
};

}  // namespace lemons::dsp::FX
