#pragma once

namespace bav::dsp
{
template < typename SampleType, typename VoiceType >
struct TemplateSynth : public SynthBase< SampleType >
{
    SynthVoiceBase< SampleType >* createVoice() final
    {
        return new VoiceType (this);
    }
};

}  // namespace bav::dsp
