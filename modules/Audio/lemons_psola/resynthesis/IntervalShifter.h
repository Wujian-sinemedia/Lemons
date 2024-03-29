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

namespace lemons::dsp::psola
{

template <typename SampleType>
class IntervalShifter final : public Shifter<SampleType>
{
public:

	using Shifter<SampleType>::Shifter;

	void setShiftingInterval (int newIntervalSemitones);

	[[nodiscard]] int getShiftingInterval() const noexcept;

private:

	void onNewBlock() final;

	void pitchHzChanged (int newPitchHz) final;

	void setPitchFromLatestAndInterval();

	int intervalSt { 0 };
};

}  // namespace lemons::dsp::psola
