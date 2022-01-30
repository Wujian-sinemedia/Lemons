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

	this->setPitchHz (this->analyzer.getLastInputPitch() * shiftingRatio);
}

template <typename SampleType>
void RatioShifter<SampleType>::pitchHzChanged (int newPitchHz)
{
	const auto inputPitch = this->analyzer.getLastInputPitch();

	if (inputPitch == 0)
		return;

	shiftingRatio = newPitchHz / inputPitch;
}

template class RatioShifter<float>;
template class RatioShifter<double>;

}  // namespace lemons::dsp::psola
