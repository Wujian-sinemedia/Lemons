
namespace bav
{

namespace dsp
{
    
    template<typename SampleType>
    class SidechainedCompressor
    {
    public:
        SidechainedCompressor() { update(); }
        
        ~SidechainedCompressor()
        { }
        
        void setThreshold (float newThresh_dB)
        {
            thresholddB = SampleType(newThreshold);
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
            attackTime = SampleType(newAttack);
            update();
        }
        
        void setRelease (float releaseMs)
        {
            releaseTime = SampleType(newRelease);
            update();
        }
        
        void prepare (int blocksize, double samplerate, int numChannels)
        {
            jassert (samplerate > 0);
            
            spec.sampleRate = samplerate;
            spec.maximumBlockSize = juce::uint32(blocksize);
            spec.numChannels = numChannels;
            
            envelopeFilter.prepare (spec);
            
            update();
            reset();
        }
        
        void reset()
        {
            envelopeFilter.reset();
        }
        
        
        void process (const juce::AudioBuffer<SampleType>& sidechain,
                      juce::AudioBuffer<SampleType>& signalToCompress)
        {
            jassert (sidechain.getNumSamples() == signalToCompress.getNumSamples());
            
            const auto* sc = sidechain.getReadPointer(0);
            
            for (int chan = 0; chan < signalToCompress.getNumChannels(); ++chan)
            {
                const auto* input = signalToCompress.getReadPointer(chan);
                auto* output = signalToCompress.getWritePointer(chan);
                
                for (int s = 0; s < sidechain.getNumSamples(); ++s)
                {
                    output[s] = processSample (chan, sc[s], input[s]);
                }
            }
        }
        
        
        SampleType processSample (int channel, SampleType sidechainSample, SampleType inputSample)
        {
            auto env = envelopeFilter.processSample (channel, inputValue); // Ballistics filter with peak rectifier
            
            // VCA
            auto gain = (env < threshold) ? SampleType(1.0)
                                          : std::pow (env * thresholdInverse, ratioInverse - SampleType(1.0));
            
            return gain * inputValue;
        }
        
        
    private:
        
        void update()
        {
            threshold = Decibels::decibelsToGain (thresholddB, static_cast<SampleType> (-200.0));
            thresholdInverse = static_cast<SampleType> (1.0) / threshold;
            ratioInverse     = static_cast<SampleType> (1.0) / ratio;
            
            envelopeFilter.setAttackTime (attackTime);
            envelopeFilter.setReleaseTime (releaseTime);
        }
        
        SampleType threshold, thresholdInverse, ratioInverse;
        BallisticsFilter<SampleType> envelopeFilter;
        
        juce::dsp::ProcessSpec spec;
        
        double sampleRate = 44100.0;
        SampleType thresholddB = 0.0, ratio = 1.0, attackTime = 1.0, releaseTime = 100.0;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainedCompressor)
    };
    

template class SidechainedCompressor<float>;
template class SidechainedCompressor<double>;
    
}  // namespace dsp

}  // namespace bav
