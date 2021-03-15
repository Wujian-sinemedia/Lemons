/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
*/


namespace bav
{

namespace dsp
{
    

template<typename SampleType>
class DeEsser
{
public:
    DeEsser() { }
    
    ~DeEsser() { }
    
    
    void prepare (int blocksize, double samplerate)
    {
        
    }
    
    
    void process (juce::AudioBuffer<SampleType>& audio)
    {
        
    }
    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeEsser)
};
    
    
}  // namespace dsp

}  // namespace bav
