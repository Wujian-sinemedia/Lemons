
namespace lemons::dsp::FX
{
template <typename SampleType>
void EQ<SampleType>::process (AudioBuffer<SampleType>& audio)
{
	jassert (getNumBands() > 0);

	for (auto* filter : bands)
		filter->process (audio);
}

template <typename SampleType>
void EQ<SampleType>::prepare (double samplerate, int blocksize)
{
	for (auto* filter : bands)
		filter->prepare (samplerate, blocksize);

	lastSamplerate = samplerate;
	lastBlocksize  = blocksize;
}

template <typename SampleType>
void EQ<SampleType>::addBand (Band* newFilter)
{
	bands.add (newFilter);
	newFilter->prepare (lastSamplerate, lastBlocksize);
}

template <typename SampleType>
int EQ<SampleType>::getNumBands() const
{
	return bands.size();
}

template <typename SampleType>
Filter<SampleType>* EQ<SampleType>::getBand (int index)
{
	if (index > bands.size()) return nullptr;
	return &bands[index]->getFilter();
}

template <typename SampleType>
Filter<SampleType>* EQ<SampleType>::getBandOfType (FilterType type)
{
	for (auto* band : bands)
	{
		auto& filter = band->getFilter();

		if (filter.getFilterType() == type)
			return &filter;
	}

	return nullptr;
}

template <typename SampleType>
Filter<SampleType>* EQ<SampleType>::getBandAtFrequency (float freq)
{
	for (auto* band : bands)
	{
		auto& filter = band->getFilter();

		if (filter.getFilterFrequency() == freq)
			return &filter;
	}

	return nullptr;
}

template <typename SampleType>
void EQ<SampleType>::serialize (TreeReflector& ref)
{
	ref.add ("Bands", bands);
}

template class EQ<float>;
template class EQ<double>;

}  // namespace lemons::dsp::FX
