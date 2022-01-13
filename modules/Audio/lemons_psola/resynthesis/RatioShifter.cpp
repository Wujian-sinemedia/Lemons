namespace lemons::dsp::psola
{

template <typename SampleType>
void RatioShifter<SampleType>::setShiftingRatio (float newRatio)
{
	jassert (newRatio > 0.f);

	shiftingRatio = newRatio;

	setPitchFromLatestAndShiftingRatio();
}

template <typename SampleType>
float RatioShifter<SampleType>::getShiftingRatio() const noexcept
{
	return shiftingRatio;
}

template <typename SampleType>
void RatioShifter<SampleType>::onNewBlock()
{
	setPitchFromLatestAndShiftingRatio();
}

template <typename SampleType>
void RatioShifter<SampleType>::setPitchFromLatestAndShiftingRatio()
{
	jassert (shiftingRatio > 0.f);

	this->setPitch (this->analyzer.getLastInputPitch() * shiftingRatio);
}

template class RatioShifter<float>;
template class RatioShifter<double>;

}  // namespace lemons::dsp::psola
