
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
        }
        
        ~Reverb() { }
        
        void prepare (double samplerate)
        {
            reverb.setSampleRate (samplerate);
            reverb.setParameters (params);
        }
        
        void reset()
        {
            reverb.reset();
        }
        
        void setRoomSize (float newRoomSize)
        {
            params.roomSize = newRoomSize;
        }
        
        void setDamping (float newDampingAmount)
        {
            params.damping = newDampingAmount;
        }
        
        void setWidth (float newWidth)
        {
            params.width = newWidth;
        }
        
        void setDryWet (int wetMixPercent)
        {
            params.wetLevel = wetMixPercent * 0.01f;
            params.dryLevel = 1.0f - params.wetLevel;
        }
        
        
        void process (juce::AudioBuffer<SampleType>& input)
        {
            switch (input.getNumChannels())
            {
                case (0): return;
                    
                case (1):
                    reverb.processMono (input.getWritePointer(0), input.getNumSamples());
                    
                default:
                    reverb.processStereo (input.getWritePointer(0),
                                          input.getWritePointer(1),
                                          input.getNumSamples());
            }
        }
        
        
    private:
        
        juce::Reverb reverb;
        
        juce::Reverb::Parameters params;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Reverb)
    };
    
}  // namespace dsp

}  // namespace bav

