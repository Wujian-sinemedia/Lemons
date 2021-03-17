
namespace bav::dsp::FX
{

    
    template<typename SampleType>
    class Limiter
    {
    public:
        Limiter() { }
        
        ~Limiter() { }
        
        void setThreshold (float thresh_dB)
        {
            thresholddB = thresh_dB;
            update();
        }
        
        void setRelease (float release_ms)
        {
            releaseTime = release_ms;
            update();
        }
        
        void prepare (int blocksize, double samplerate, int numChannels)
        {
            jassert (samplerate > 0);
            jassert (numChannels > 0);
            
            sampleRate = samplerate;
            
            firstStageCompressor.prepare (blocksize, samplerate, numChannels);
            secondStageCompressor.prepare (blocksize, samplerate, numChannels);
            
            update();
            reset();
        }
        
        void reset()
        {
            firstStageCompressor.reset();
            secondStageCompressor.reset();
            
            outputVolume.reset (sampleRate, 0.001);
        }
        
        
        void process (const juce::AudioBuffer<SampleType>& sidechain,
                      juce::AudioBuffer<SampleType>& signalToLimit)
        {
            const int numChannels = signalToLimit.getNumChannels();
            const int numSamples  = signalToLimit.getNumSamples();
            
            jassert (sidechain.getNumChannels() == numChannels);
            jassert (sidechain.getNumSamples() == numSamples);
            
            for (int channel = 0; channel < numChannels; ++channel)
            {
                process (channel,
                         sidechain.getReadPointer (channel),
                         signalToLimit.getWritePointer (channel),
                         numSamples);
            }
        }
        
        
        void process (juce::AudioBuffer<SampleType>& signalToLimit)
        {
            for (int channel = 0; channel < signalToLimit.getNumChannels(); ++channel)
                process (channel, nullptr, signalToLimit.getWritePointer (channel), numSamples);
        }
        
        
        void process (const int channel,
                      const SampleType* sidechain = nullptr,
                      SampleType* signalToLimit,
                      const int numSamples)
        {
            if (sidechain == nullptr)
                sidechain = signalToLimit;
            
            for (int s = 0; s < numSamples; ++s)
            {
                const auto sc = sidechain[s];
                *(signalToLimit + s) = firstStageCompressor.processSample  (channel, sc, signalToLimit[s]);
                *(signalToLimit + s) = secondStageCompressor.processSample (channel, sc, signalToLimit[s]);
                *(signalToLimit + s) = signalToLimit[s] * outputVolume.getNextValue();
            }
            
            juce::FloatVectorOperations::clip (signalToLimit, signalToLimit, SampleType(-1.0), SampleType(1.0), numSamples);
        }
        
        
    private:
        
        void update()
        {
            firstStageCompressor.setThreshold (-10.0f);
            firstStageCompressor.setRatio     (4.0f);
            firstStageCompressor.setAttack    (2.0f);
            firstStageCompressor.setRelease   (200.0f);
            
            secondStageCompressor.setThreshold (thresholddB);
            secondStageCompressor.setRatio     (1000.0f);
            secondStageCompressor.setAttack    (0.001f);
            secondStageCompressor.setRelease   (releaseTime);
            
            auto ratioInverse = (SampleType) (1.0 / 4.0);
            
            auto gain = (SampleType) std::pow (10.0, 10.0 * (1.0 - ratioInverse) / 40.0);
            gain *= juce::Decibels::decibelsToGain (-thresholddB, -100.0f);
            
            outputVolume.setTargetValue (gain);
        }
        
        
        Compressor<SampleType> firstStageCompressor, secondStageCompressor;
        
        juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Linear> outputVolume;
        
        double sampleRate = 44100.0;
        float thresholddB = -10.0, releaseTime = 100.0;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Limiter)
    };
    
    
    template class Limiter<float>;
    template class Limiter<double>;
    
}  // namespace bav

