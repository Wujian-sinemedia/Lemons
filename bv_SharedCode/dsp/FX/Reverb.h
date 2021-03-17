

namespace bav::dsp::FX
{
    
    class Reverb
    {
    public:
        Reverb()
        {
            params.roomSize = 0.5f;
            params.damping = 0.35f;
            params.wetLevel = 1.0f;
            params.dryLevel = 0.0f;
            params.width = 1.0f;
            params.freezeMode = 0.2f;
            
            compressor.setAttack (15.0f);
            compressor.setRelease (35.0f);
        }
        
        ~Reverb() { }
        
        void prepare (int blocksize, double samplerate, int numChannels)
        {
            jassert (numChannels <= 2);
            jassert (samplerate > 0 && blocksize > 0 && numChannels > 0);
            
            reverb.setSampleRate (samplerate);
            reverb.setParameters (params);
            
            compressor.prepare (blocksize, samplerate, 2);
            
            sampleRate = samplerate;
            
            loCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (samplerate, loCutFreq);
            loCut.reset();
            
            hiCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (samplerate, hiCutFreq);
            hiCut.reset();
            
            sidechainBuffer.setSize (numChannels, blocksize, true, true, true);
            workingBuffer.setSize (numChannels, blocksize, true, true, true);
            conversionBuffer.setSize (numChannels, blocksize, true, true, true);
            
            dryGain.reset (blocksize);
            wetGain.reset (blocksize);
        }
        
        void reset()
        {
            reverb.reset();
            compressor.reset();
            workingBuffer.clear();
            sidechainBuffer.clear();
            conversionBuffer.clear();
            
            const int numSamps = workingBuffer.getNumSamples();
            dryGain.reset (numSamps);
            wetGain.reset (numSamps);
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
            const auto wet = wetMixPercent * 0.01f;
            wetGain.setTargetValue (wet);
            dryGain.setTargetValue (1.0f - wet);
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
        
        
        //  process input with no external compressor sidechain
        void process (juce::AudioBuffer<double>& input)
        {
            process (input, input);
        }
        
        void process (juce::AudioBuffer<float>& input)
        {
            process (input, input);
        }
        
        
        //  process input with an external signal as the compressor's sidechain
        
        void process (juce::AudioBuffer<double>& input,
                      juce::AudioBuffer<double>& compressorSidechain)
        {
            conversionBuffer.makeCopyOf (input, true);
            sidechainBuffer.makeCopyOf (*compressorSidechain, true);
            
            process (conversionBuffer, &sidechainBuffer);
        
            input.makeCopyOf (conversionBuffer, true);
        }
        
        
        void process (juce::AudioBuffer<float>& input,
                      juce::AudioBuffer<float>& compressorSidechain)
        {
            const int numSamples = input.getNumSamples();
            const int numChannels = input.getNumChannels();
            
            jassert (numSamples == compressorSidechain.getNumSamples());
            jassert (numChannels == compressorSidechain.getNumChannels());
            
            sidechainBuffer.makeCopyOf (compressorSidechain, true);
            workingBuffer.makeCopyOf (input);
            
            // reverb
            switch (numChannels)
            {
                case (0): return;
                    
                case (1):
                    reverb.processMono (workingBuffer.getWritePointer(0), numSamples);
                    
                default:
                    reverb.processStereo (workingBuffer.getWritePointer(0), workingBuffer.getWritePointer(1), numSamples);
            }
            
            // filters
            for (int chan = 0; chan < numChannels; ++chan)
            {
                juce::AudioBuffer<float> mono (workingBuffer.getArrayOfWritePointers() + chan, 1, 0, numSamples);
                juce::dsp::AudioBlock<float> block (mono);
                juce::dsp::ProcessContextReplacing<float> context (block);
                loCut.process (context);
                hiCut.process (context);
            }
            
            // sidechain compressor
            if (isDucking)
                compressor.process (sidechainBuffer, workingBuffer);
            
            // write to output & apply dry/wet gain
            input.clear();
            
            for (int chan = 0; chan < numChannels; ++chan)
            {
                auto* output = input.getWritePointer(chan);
                const auto* drySamples = sidechainBuffer.getReadPointer(chan);
                const auto* wetSamples = workingBuffer.getReadPointer(chan);
                
                for (int s = 0; s < numSamples; ++s)
                    output[s] = (drySamples[s] * dryGain.getNextValue()) + (wetSamples[s] * wetGain.getNextValue());
            }
        }
        
        
    private:
        
        juce::Reverb reverb;
        
        juce::Reverb::Parameters params;
        
        juce::AudioBuffer<float> conversionBuffer;
        juce::AudioBuffer<float> workingBuffer;
        juce::AudioBuffer<float> sidechainBuffer;
        
        bool isDucking;
        Compressor<float> compressor;
        
        juce::dsp::IIR::Filter<float> loCut, hiCut;
        float loCutFreq = 80.0f, hiCutFreq = 5500.0f;
        
        double sampleRate = 0.0;
        
        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> dryGain;
        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> wetGain;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Reverb)
    };
    
    
}  // namespace







