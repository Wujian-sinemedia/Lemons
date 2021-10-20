#pragma once

namespace lemons::dsp
{
template <typename SampleType, typename VoiceType>
struct TemplateSynth : public SynthBase<SampleType>
{
	SynthVoiceBase<SampleType>* createVoice() final
	{
		return new VoiceType (this);
	}
};

}  // namespace lemons::dsp
