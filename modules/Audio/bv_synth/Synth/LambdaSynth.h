#pragma once


namespace bav::dsp
{
template < typename SampleType >
class LambdaSynth : public SynthBase< SampleType >
{
public:
    using Voice             = SynthVoiceBase< SampleType >;
    using VoiceCreationFunc = std::function< Voice*() >;

    LambdaSynth (VoiceCreationFunc&& creationFuncToUse)
        : createVoiceFunc (std::move (creationFuncToUse))
    {
    }

private:
    Voice* createVoice() final
    {
        return createVoiceFunc();
    }

    VoiceCreationFunc createVoiceFunc;
};


template < typename SampleType, typename VoiceType >
struct TemplateSynth : public SynthBase< SampleType >
{
    SynthVoiceBase< SampleType >* createVoice() final
    {
        return new VoiceType (this);
    }
};

}  // namespace bav::dsp
