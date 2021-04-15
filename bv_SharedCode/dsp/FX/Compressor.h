

namespace bav::dsp::FX
{
    
/*
    Simple compressor that allows you to sidechain the signal.
*/
    
    template<typename SampleType>
    class Compressor
    {
    public:
        Compressor() { update(); }
        
        ~Compressor()
        { }
        
        void setThreshold (float newThresh_dB)
        {
            thresholddB = SampleType(newThresh_dB);
            update();
        }
        
        void setRatio (float newRatio)
        {
            jassert (newRatio >= 1.0f);
            
            ratio = SampleType(newRatio);
            update();
        }
        
        void setAttack (float attackMs)
        {
            attackTime = SampleType(attackMs);
            update();
        }
        
        void setRelease (float releaseMs)
        {
            releaseTime = SampleType(releaseMs);
            update();
        }
        
        void prepare (int blocksize, double samplerate, int numChannels)
        {
            jassert (samplerate > 0);
            
            spec.sampleRate = samplerate;
            spec.maximumBlockSize = juce::uint32(blocksize);
            spec.numChannels = juce::uint32(numChannels);
            
            envelopeFilter.prepare (spec);
            
            update();
            reset();
        }
        
        void reset()
        {
            envelopeFilter.reset();
        }
        
        
        //  processes a signal with no external sidechain
        void process (juce::AudioBuffer<SampleType>& signalToCompress,
                      SampleType* gainReduction = nullptr)
        {
            process (signalToCompress, signalToCompress, gainReduction);
        }
        
        
        // processes a signal with an external sidechain. Use the same buffer in both arguments to sidechain a signal to itself.
        void process (const juce::AudioBuffer<SampleType>& sidechain,
                      juce::AudioBuffer<SampleType>& signalToCompress,
                      SampleType* gainReduction = nullptr)
        {
            const auto numChannels = signalToCompress.getNumChannels();
            const auto numSamples  = signalToCompress.getNumSamples();
            
            jassert (sidechain.getNumChannels() == numChannels);
            jassert (sidechain.getNumSamples() == numSamples);
            
            for (int chan = 0; chan < numChannels; ++chan)
            {
                process (chan, numSamples,
                         signalToCompress.getWritePointer (chan),
                         sidechain.getReadPointer (chan),
                         gainReduction);
            }
        }
        
        
        //  processes a signal with an external sidechain. Omit the sidechain argument or pass a nullptr to sidechain the signal to itself.
        void process (const int channel,
                      const int numSamples,
                      SampleType* signalToCompress,
                      const SampleType* sidechain = nullptr,
                      SampleType* gainReduction = nullptr)
        {
            jassert (numSamples > 0);
            
            if (sidechain == nullptr)
                sidechain = signalToCompress;
            
            SampleType avgGainReduction = 0;
            SampleType gainRedux = 0;
            
            for (int s = 0; s < numSamples; ++s)
            {
                *(signalToCompress + s) = processSample (channel, signalToCompress[s], sidechain[s], &gainRedux);
                avgGainReduction += gainRedux;
            }
            
            if (gainReduction != nullptr)
            {
                avgGainReduction *= (1 / numSamples);
                *gainReduction = avgGainReduction;
            }
        }
        
        
        //  processes a single sample with no external sidechain
        SampleType processSample (int channel,
                                  SampleType inputSample,
                                  SampleType* gainReduction = nullptr)
        {
            return processSample (channel, inputSample, inputSample, gainReduction);
        }
        
        
        //  processes a single sample. Omit the sidechainSample argument or pass the same value in both args to use as a standard compressor (sidechaining the signal to itself).
        SampleType processSample (int channel,
                                  SampleType inputSample,
                                  SampleType sidechainSample,
                                  SampleType* gainReduction = nullptr)
        {
            auto env = envelopeFilter.processSample (channel, sidechainSample); // Ballistics filter with peak rectifier
            
            // VCA
            auto gain = (env < threshold) ? SampleType(1.0)
                                          : std::pow (env * thresholdInverse, ratioInverse - SampleType(1.0));
            
            if (gainReduction != nullptr)  // report gain reduction, if requested
                *gainReduction = gain;
            
            return gain * inputSample;
        }
    
        
        
    private:
        
        void update()
        {
            threshold = juce::Decibels::decibelsToGain (thresholddB, static_cast<SampleType> (-200.0));
            thresholdInverse = static_cast<SampleType> (1.0) / threshold;
            ratioInverse     = static_cast<SampleType> (1.0) / ratio;
            
            envelopeFilter.setAttackTime (attackTime);
            envelopeFilter.setReleaseTime (releaseTime);
        }
        
        SampleType threshold, thresholdInverse, ratioInverse;
        juce::dsp::BallisticsFilter<SampleType> envelopeFilter;
        
        juce::dsp::ProcessSpec spec;
        
        double sampleRate = 44100.0;
        SampleType thresholddB = 0.0, ratio = 1.0, attackTime = 1.0, releaseTime = 100.0;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compressor)
    };
    
    
    template class Compressor<float>;
    template class Compressor<double>;
    
    
    /*
        A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
    */
    template<typename SampleType>
    class ReorderableCompressor :    public Compressor,
                                     public ReorderableEffect
    {
        using Compressor = Compressor<SampleType>;
        
    public:
        ReorderableCompressor() { }
    
    protected:
        void fxChain_process (juce::AudioBuffer<SampleType>& audio) override
        {
            Compressor::process (audio, nullptr);
        }
        
        void fxChain_prepare (double samplerate, int blocksize) override
        {
            Compressor::prepare (blocksize, samplerate, 2);
        }
    
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReorderableCompressor)
    };
    
    template class ReorderableCompressor<float>;
    template class ReorderableCompressor<double>;
    
}  // namespace bav

