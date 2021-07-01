
#pragma once

namespace bav::dsp
{

template<typename SampleType,
         template<typename NumericType> class Voice,
         std::enable_if_t< std::is_base_of< SynthVoiceBase<SampleType>, Voice<SampleType> >::value >* = nullptr >
struct Synth : SynthBase< SampleType >
{
    SynthVoiceBase<SampleType>* createVoice() final
    {
        return new Voice<SampleType>(this);
    }
};

}
