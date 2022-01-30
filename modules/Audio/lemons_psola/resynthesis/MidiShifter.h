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
class MidiShifter final : public Shifter<SampleType>
{
public:

	using Shifter<SampleType>::Shifter;

	void setMidiPitch (float newMidiPitch);

	[[nodiscard]] float getMidiPitch() const noexcept;

private:

	void onNewBlock() final;

	void pitchHzChanged (int newPitchHz) final;

	void setPitchFromMidiPitch();

	float midiPitch { 69.f };
};

}  // namespace lemons::dsp::psola
