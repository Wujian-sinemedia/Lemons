
/** @ingroup BasicSynths
 *  @{
 */
#pragma once


namespace lemons::dsp
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

}  // namespace lemons::dsp

/** @}*/
