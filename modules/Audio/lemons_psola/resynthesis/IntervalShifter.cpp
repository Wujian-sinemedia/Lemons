namespace lemons::dsp::psola
{

template <typename SampleType>
void IntervalShifter<SampleType>::setShiftingInterval (int newIntervalSemitones)
{
	intervalSt = newIntervalSemitones;
	setPitchFromLatestAndInterval();
}

template <typename SampleType>
int IntervalShifter<SampleType>::getShiftingInterval() const noexcept
{
	return intervalSt;
}

template <typename SampleType>
void IntervalShifter<SampleType>::onNewBlock()
{
	setPitchFromLatestAndInterval();
}

template <typename SampleType>
void IntervalShifter<SampleType>::setPitchFromLatestAndInterval()
{
	const auto inputMidi = math::freqToMidi (this->analyzer.getLastInputPitch());

	this->setPitch (math::midiToFreq (inputMidi + intervalSt));
}

template class IntervalShifter<float>;
template class IntervalShifter<double>;

}  // namespace lemons::dsp::psola
