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
	const auto inputPitch = this->analyzer.getLastInputPitch();

	if (inputPitch == 0)
		return;

	const auto inputMidi = math::freqToMidi (inputPitch);

	this->setPitchHz (math::midiToFreq (inputMidi + intervalSt));
}

template <typename SampleType>
void IntervalShifter<SampleType>::pitchHzChanged (int newPitchHz)
{
	const auto inputPitch = this->analyzer.getLastInputPitch();

	if (inputPitch == 0)
		return;

	const auto inputMidi = math::freqToMidi (inputPitch);
	const auto thisMidi	 = math::freqToMidi (newPitchHz);

	intervalSt = thisMidi - inputMidi;
}

template class IntervalShifter<float>;
template class IntervalShifter<double>;

}  // namespace lemons::dsp::psola
