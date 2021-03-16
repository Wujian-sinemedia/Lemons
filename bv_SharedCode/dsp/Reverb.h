
namespace bav
{

namespace dsp
{
    
    template<typename SampleType>
    class Reverb
    {
    public:
        Reverb()
        {
            params.roomSize = 0.5f;
            params.damping = 0.35f;
            params.wetLevel = 0.4f;
            params.dryLevel = 0.6f;
            params.width = 1.0f;
            params.freezeMode = 0.2f;
            
            compressor.setAttack (15.0f);
            compressor.setRelease (35.0f);
        }
        
        ~Reverb() { }
        
        void prepare (int blocksize, double samplerate)
        {
            reverb.setSampleRate (samplerate);
            reverb.setParameters (params);
            
            compressor.prepare (blocksize, samplerate, 2);
            
            sampleRate = samplerate;
            
            loCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (samplerate, loCutFreq);
            loCut.reset();
            
            hiCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (samplerate, hiCutFreq);
            hiCut.reset();
            
            if constexpr (std::is_same_v <SampleType, double>)
            {
                workingBuffer.setSize (2, blocksize, true, true, true);
                sidechainBuffer.setSize (2, blocksize, true, true, true);
            }
        }
        
        void reset()
        {
            reverb.reset();
            compressor.reset();
            workingBuffer.clear();
        }
        
        void setRoomSize (float newRoomSize)
        {
            params.roomSize = newRoomSize;
            reverb.setParameters (params);
        }
        
        void setDamping (float newDampingAmount)
        {
            params.damping = newDampingAmount;
            reverb.setParameters (params);
        }
        
        void setWidth (float newWidth)
        {
            params.width = newWidth;
            reverb.setParameters (params);
        }
        
        void setDryWet (int wetMixPercent)
        {
            params.wetLevel = wetMixPercent * 0.01f;
            params.dryLevel = 1.0f - params.wetLevel;
            reverb.setParameters (params);
        }
        
        void setDuckAmount (float newDuckAmount)
        {
            isDucking = newDuckAmount > 0.0f;
            compressor.setThreshold (juce::jmap (newDuckAmount, 0.0f, -20.0f));
            compressor.setRatio (juce::jmap (newDuckAmount, 1.0f, 10.0f));
        }
        
        void setLoCutFrequency (float freq)
        {
            loCutFreq = freq;
            loCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, loCutFreq);
            loCut.reset();
        }
        
        void setHiCutFrequency (float freq)
        {
            hiCutFreq = freq;
            hiCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, hiCutFreq);
            hiCut.reset();
        }
        
        
        void process (juce::AudioBuffer<double>& input,
                      juce::AudioBuffer<double>* compressorSidechain = nullptr)
        {
            workingBuffer.makeCopyOf (input, true);
            
            if (compressorSidechain == nullptr)
            {
                process (workingBuffer, nullptr);
            }
            else
            {
                sidechainBuffer.makeCopyOf (*compressorSidechain, true);
                process (workingBuffer, &sidechainBuffer);
            }
            
            input.makeCopyOf (workingBuffer, true);
        }
        
        
        void process (juce::AudioBuffer<float>& input,
                      juce::AudioBuffer<float>* compressorSidechain = nullptr)
        {
            const int numSamples = input.getNumSamples();
            
            if (compressorSidechain != nullptr)
                jassert (numSamples == compressorSidechain->getNumSamples());
            
            switch (input.getNumChannels())
            {
                case (0): return;
                    
                case (1):
                    reverb.processMono (input.getWritePointer(0), numSamples);
                    
                default:
                    reverb.processStereo (input.getWritePointer(0),
                                          input.getWritePointer(1),
                                          numSamples);
            }
            
            for (int chan = 0; chan < 2; ++chan)
            {
                juce::AudioBuffer<float> mono (input.getArrayOfWritePointers() + chan, 1, 0, numSamples);
                juce::dsp::AudioBlock<float> block (mono);
                juce::dsp::ProcessContextReplacing<float> context (block);
                loCut.process (context);
                hiCut.process (context);
            }
            
            if (isDucking)
            {
                if (compressorSidechain == nullptr)
                    compressor.process (input, input);
                else
                    compressor.process (*compressorSidechain, input);
            }
        }
        
        
    private:
        
        juce::Reverb reverb;
        
        juce::Reverb::Parameters params;
        
        juce::AudioBuffer<float> workingBuffer;
        juce::AudioBuffer<float> sidechainBuffer;
        
        bool isDucking;
        bav::dsp::SidechainedCompressor<float> compressor;
        
        juce::dsp::IIR::Filter<float> loCut, hiCut;
        float loCutFreq = 80.0f, hiCutFreq = 5500.0f;
        
        double sampleRate = 0.0;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Reverb)
    };
    
    
    template class Reverb<float>;
    template class Reverb<double>;
    
}  // namespace dsp

}  // namespace bav




