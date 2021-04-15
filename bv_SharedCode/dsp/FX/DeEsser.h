/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */

/*
    Simple de-esser that works by sending the input signal through an inverted noise gate sidechained by a hi-passed version of the original signal.
*/

namespace bav::dsp::FX
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
    
    
    void process (juce::AudioBuffer<SampleType>& audio, SampleType* gainReduction = nullptr)
    {
        const auto numSamples = audio.getNumSamples();
        
        for (int chan = 0; chan < std::min(2, audio.getNumChannels()); ++chan)
            process (chan, audio.getWritePointer(chan), numSamples, gainReduction);
    }
    
    
    void process (const int channel,
                  SampleType* signalToDeEss,
                  const int numSamples,
                  SampleType* gainReduction = nullptr)
    {
        SampleType avgGainReduction = 0;
        SampleType gainRedux = 0;
        
        for (int s = 0; s < numSamples; ++s)
        {
            *(signalToDeEss + s) = processSample (channel, signalToDeEss[s], &gainRedux);
            avgGainReduction += gainRedux;
        }
        
        if (gainReduction != nullptr)
        {
            avgGainReduction *= (1 / numSamples);
            *gainReduction = avgGainReduction;
        }
    }
    
    
    SampleType processSample (const int channel,
                              const SampleType inputSample,
                              SampleType* gainReduction = nullptr)
    {
        return gate.processSample (channel,
                                   inputSample,
                                   hiPass.processSample (inputSample),
                                   gainReduction);
    }
    

private:
    
    juce::dsp::IIR::Filter<SampleType> hiPass;
    
    NoiseGate<SampleType> gate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeEsser)
};


template class DeEsser<float>;
template class DeEsser<double>;
 
 
    /*
        A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
    */
    template<typename SampleType>
    class ReorderableDeEsser :    public DeEsser<SampleType>,
                                  public ReorderableEffect<SampleType>
    {
        using DeEsser = DeEsser<SampleType>;
        
    public:
        ReorderableDeEsser() { }
    
    protected:
        void fxChain_process (juce::AudioBuffer<SampleType>& audio) override
        {
            DeEsser::process (audio, nullptr);
        }
        
        void fxChain_prepare (double samplerate, int blocksize) override
        {
            DeEsser::prepare (blocksize, samplerate);
        }
    
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReorderableDeEsser)
    };
    
    template class ReorderableDeEsser<float>;
    template class ReorderableDeEsser<double>;

}  // namespace

