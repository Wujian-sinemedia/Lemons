#pragma once

namespace lemons::dsp::psola
{

template <typename SampleType>
class RatioShifter final : public Shifter<SampleType>
{
public:

	using Shifter<SampleType>::Shifter;

	void setShiftingRatio (float newRatio);

	[[nodiscard]] float getShiftingRatio() const noexcept;

private:

	void onNewBlock() final;

	void pitchHzChanged (int newPitchHz) final;

	void setPitchFromLatestAndShiftingRatio();

	float shiftingRatio { 1.f };
};

}  // namespace lemons::dsp::psola
