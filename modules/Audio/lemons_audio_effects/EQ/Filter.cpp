
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

namespace lemons::dsp::FX
{
template <typename SampleType>
Filter<SampleType>::Filter (FilterType filterType, float frequency, float Qfactor, float gainMult)
	: type (filterType), freq (static_cast<SampleType> (frequency)), Q (static_cast<SampleType> (Qfactor)), gain (static_cast<SampleType> (gainMult))
{
	filter.prepare();
}

template <typename SampleType>
void Filter<SampleType>::process (juce::AudioBuffer<SampleType>& audio)
{
	switch (type)
	{
		case (FilterType::LowPass) :
		{
			filter.coefs.makeLowPass (sampleRate, freq, Q);
			break;
		}
		case (FilterType::HighPass) :
		{
			filter.coefs.makeHighPass (sampleRate, freq, Q);
			break;
		}
		case (FilterType::LowShelf) :
		{
			filter.coefs.makeLowShelf (sampleRate, freq, Q, gain);
			break;
		}
		case (FilterType::HighShelf) :
		{
			filter.coefs.makeHighShelf (sampleRate, freq, Q, gain);
			break;
		}
		case (FilterType::BandPass) :
		{
			filter.coefs.makeBandPass (sampleRate, freq, Q);
			break;
		}
		case (FilterType::Notch) :
		{
			filter.coefs.makeNotch (sampleRate, freq, Q);
			break;
		}
		case (FilterType::Peak) :
		{
			filter.coefs.makePeakFilter (sampleRate, freq, Q, gain);
			break;
		}
		case (FilterType::AllPass) :
		{
			filter.coefs.makeAllPass (sampleRate, freq, Q);
			break;
		}
	}

	filter.process (audio);
}

template <typename SampleType>
void Filter<SampleType>::prepare (double samplerate, int)
{
	sampleRate = samplerate;
	filter.prepare();
}

template <typename SampleType>
void Filter<SampleType>::setFilterType (FilterType newType)
{
	type = newType;
}

template <typename SampleType>
FilterType Filter<SampleType>::getFilterType() const
{
	return type;
}

template <typename SampleType>
void Filter<SampleType>::setFilterFrequency (float newFreqHz)
{
	freq = static_cast<SampleType> (newFreqHz);
}

template <typename SampleType>
float Filter<SampleType>::getFilterFrequency() const
{
	return static_cast<float> (freq);
}

template <typename SampleType>
void Filter<SampleType>::setQfactor (float newQ)
{
	jassert (newQ > 0.f);
	Q = static_cast<SampleType> (newQ);
}

template <typename SampleType>
float Filter<SampleType>::getQfactor() const
{
	return static_cast<float> (Q);
}

template <typename SampleType>
void Filter<SampleType>::setGain (float newGain)
{
	gain = static_cast<SampleType> (newGain);
}

template <typename SampleType>
float Filter<SampleType>::getGain() const
{
	return static_cast<float> (gain);
}

template class Filter<float>;
template class Filter<double>;


template struct FilterEngine<float>;
template struct FilterEngine<double>;

}  // namespace lemons::dsp::FX
