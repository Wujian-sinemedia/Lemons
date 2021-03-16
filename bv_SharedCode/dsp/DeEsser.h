/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */


namespace bav
{

namespace dsp
{
    

#define bvng_HI_PASS_FREQ 7600.0

#define bvng_ATTACK_MS 20
#define bvng_RELEASE_MS 30


template<typename SampleType>
class DeEsser
{
public:
    DeEsser()
    {
        gate.setAttack (bvng_ATTACK_MS);
        gate.setRelease (bvng_RELEASE_MS);
        gate.setInverted (true);
    }
    
    ~DeEsser() { }
    
    
    void prepare (int blocksize, double samplerate)
    {
        hiPass.coefficients = juce::dsp::IIR::Coefficients<SampleType>::makeHighPass (samplerate,
                                                                                      SampleType(bvng_HI_PASS_FREQ));
        gate.prepare (2, blocksize, samplerate);
    }
    
#undef bvng_HI_PASS_FREQ
#undef bvng_ATTACK_MS
#undef bvng_RELEASE_MS
    
    
    void reset()
    {
        hiPass.reset();
        gate.reset();
    }
    
    
    void setThresh (float newThresh_dB)
    {
        gate.setThreshold (SampleType(newThresh_dB));
    }
    
    
    // sets the de-essing amount, from 0.0 to 1.0
    void setDeEssAmount (float newAmount)
    {
        jassert (newAmount >= 0.0f && newAmount <= 1.0f);
        gate.setRatio (SampleType (juce::jmap (newAmount, 0.0f, 1.0f, 1.0f, 10.0f)));
    }
    
    
    void process (juce::AudioBuffer<SampleType>& audio)
    {
        for (int chan = 0; chan < std::min(2, audio.getNumChannels()); ++chan)
        {
            const auto* input = audio.getReadPointer (chan);
            auto* output = audio.getWritePointer (chan);
            
            for (int s = 0; s < audio.getNumSamples(); ++s)
                output[s] = processSample (chan, input[s]);
        }
    }
    
    
    SampleType processSample (const int channel, const SampleType inputSample)
    {
        return gate.processSample (channel,
                                   hiPass.processSample (inputSample),
                                   inputSample);
    }
    

private:
    
    juce::dsp::IIR::Filter<SampleType> hiPass;
    
    SidechainedNoiseGate<SampleType> gate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeEsser)
};


template class DeEsser<float>;
template class DeEsser<double>;

}  // namespace dsp

}  // namespace bav

