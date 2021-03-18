

namespace bav::dsp::FX
{
    
    /*
        Simple noise gate that allows you to sidechain the signal.
    */
    
    template<typename SampleType>
    class NoiseGate
    {
    public:
        NoiseGate()
        {
            update();
            
            RMSFilter.setLevelCalculationType (juce::dsp::BallisticsFilterLevelCalculationType::RMS);
            RMSFilter.setAttackTime  (static_cast<SampleType> (0.0));
            RMSFilter.setReleaseTime (static_cast<SampleType> (50.0));
        }
        
        ~NoiseGate() { }
        
        void setInverted (bool gateBehaviorShouldBeInverted)
        {
            inverted = gateBehaviorShouldBeInverted;
        }
        
        
        void setThreshold (SampleType newThreshold_dB)
        {
            thresholddB = newThreshold_dB;
            update();
        }
        
        /** Sets the ratio of the noise-gate (must be higher or equal to 1).*/
        void setRatio (SampleType newRatio)
        {
            jassert (newRatio >= static_cast<SampleType> (1.0));
            
            ratio = newRatio;
            update();
        }
        
        
        void setAttack (SampleType newAttack_ms)
        {
            attackTime = newAttack_ms;
            update();
        }
        
        
        void setRelease (SampleType newRelease_ms)
        {
            releaseTime = newRelease_ms;
            update();
        }
        
        
        void prepare (int numChannels, int maxBlocksize, double samplerate)
        {
            jassert (samplerate > 0);
            jassert (numChannels > 0);
            
            spec.sampleRate = samplerate;
            spec.maximumBlockSize = juce::uint32(maxBlocksize);
            spec.numChannels = juce::uint32(numChannels);
            
            RMSFilter.prepare (spec);
            envelopeFilter.prepare (spec);
            
            update();
            reset();
        }
        
        
        void reset()
        {
            RMSFilter.reset();
            envelopeFilter.reset();
        }
        
        
        //  processes a signal with no external sidechain
        void process (juce::AudioBuffer<SampleType>& signalToGate,
                      SampleType* gainReduction = nullptr)
        {
            process (signalToGate, signalToGate, gainReduction);
        }
        
        
        // processes a signal with an external sidechain. Use the same buffer in both arguments to sidechain a signal to itself.
        void process (const juce::AudioBuffer<SampleType>& sidechain,
                      juce::AudioBuffer<SampleType>& signalToGate,
                      SampleType* gainReduction = nullptr)
        {
            const int numChannels = signalToGate.getNumChannels();
            const int numSamples  = signalToGate.getNumSamples();
            
            jassert (sidechain.getNumChannels() == numChannels);
            jassert (sidechain.getNumSamples() == numSamples);
            
            for (int channel = 0; channel < numChannels; ++channel)
            {
                process (channel, numSamples,
                         signalToGate.getWritePointer (channel),
                         sidechain.getReadPointer (channel),
                         gainReduction);
            }
        }
        
        
        //  processes a signal with an external sidechain. Omit the sidechain argument or pass a nullptr to sidechain the signal to itself.
        void process (const int channel,
                      const int numSamples,
                      SampleType* signalToGate,
                      const SampleType* sidechain = nullptr,
                      SampleType* gainReduction = nullptr)
        {
            jassert (numSamples > 0);
            
            if (sidechain == nullptr)
                sidechain = signalToGate;
            
            SampleType avgGainReduction = 0;
            SampleType gainRedux = 0;
            
            for (int s = 0; s < numSamples; ++s)
            {
                *(signalToGate + s) = processSample (channel, signalToGate[s], sidechain[s], &gainRedux);
                avgGainReduction += gainRedux;
            }
            
            if (gainReduction != nullptr)
            {
                avgGainReduction *= (1 / numSamples);
                *gainReduction = avgGainReduction;
            }
        }
        
        
        //  processes a single sample with no external sidechain
        SampleType processSample (const int channel,
                                  const SampleType sampleToGate,
                                  SampleType* gainReduction = nullptr)
        {
            return processSample (channel, sampleToGate, sampleToGate, gainReduction);
        }
        
        
        //  processes a single sample. Omit the sidechainSample argument or pass the same value in both args to use as a standard compressor (sidechaining the signal to itself).
        SampleType processSample (const int channel,
                                  const SampleType sampleToGate,
                                  const SampleType sidechainValue,
                                  SampleType* gainReduction = nullptr)
        {
            auto env = RMSFilter.processSample (channel, sidechainValue);  // RMS ballistics filter
            
            env = envelopeFilter.processSample (channel, env);  // Ballistics filter
            
            // VCA
            SampleType gain;
            
            if (inverted)
            {
                gain = (env < threshold) ? static_cast<SampleType> (1.0)
                                         : std::pow (env * thresholdInverse, currentRatio - static_cast<SampleType> (1.0));
            }
            else
            {
                gain = (env > threshold) ? static_cast<SampleType> (1.0)
                                         : std::pow (env * thresholdInverse, currentRatio - static_cast<SampleType> (1.0));
            }
            
            if (gainReduction != nullptr)  // report gain reduction, if requested
                *gainReduction = gain;
            
            return gain * sampleToGate;
        }
        
        
        
    private:
        
        void update()
        {
            threshold = juce::Decibels::decibelsToGain (thresholddB, static_cast<SampleType> (-200.0));
            thresholdInverse = static_cast<SampleType> (1.0) / threshold;
            currentRatio = ratio;
            
            envelopeFilter.setAttackTime  (attackTime);
            envelopeFilter.setReleaseTime (releaseTime);
        }
        
        
        juce::dsp::ProcessSpec spec;
        
        SampleType threshold, thresholdInverse, currentRatio;
        juce::dsp::BallisticsFilter<SampleType> envelopeFilter, RMSFilter;
        
        SampleType thresholddB = -100, ratio = 10.0, attackTime = 1.0, releaseTime = 100.0;
        
        bool inverted = false;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGate)
    };
    
    
    template class NoiseGate<float>;
    template class NoiseGate<double>;
    
}  // namespace

