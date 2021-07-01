
#pragma once

namespace bav::dsp
{

template<typename SampleType,
         template<typename NumericType> class Voice,
         std::enable_if_t< std::is_base_of< SynthVoiceBase<float>, Voice<float> >::value >* = nullptr >
struct Synth : SynthBase< SampleType >
{
    using SynthBase< SampleType >::SynthBase;
    
    SynthVoiceBase<SampleType>* createVoice() final
    {
        return new Voice<SampleType> (this);
    }
};

}
