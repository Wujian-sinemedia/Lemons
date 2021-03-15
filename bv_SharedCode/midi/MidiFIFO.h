/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */


namespace bav
{

namespace midi
{
    

class MidiFIFO
{
    
public:
    
    MidiFIFO() { }
    
    MidiFIFO(int maxNumMessages)
    {
        setSize (maxNumMessages);
    }
    
    ~MidiFIFO() { }
    
    
    void setSize (int maxNumMessages)
    {
        const size_t messages = size_t(maxNumMessages);
        base.ensureSize (messages);
        copying.ensureSize (messages);
    }
    
    
    void clear()
    {
        base.clear();
        copying.clear();
        numStoredSamples = 0;
    }
    
    
    int numStoredEvents() { return base.getNumEvents(); }
    
    
    void pushEvents (const juce::MidiBuffer& source, const int numSamples)
    {
        base.addEvents (source, 0, numSamples, numStoredSamples);
        numStoredSamples += numSamples;
    }
    
    
    void popEvents (juce::MidiBuffer& output, const int numSamples)
    {
        output.clear();
        output.addEvents (base, 0, numSamples, 0);
        
        // Move all the remaining events forward by the number of samples removed
        copying.clear();
        copying.addEvents (base, numSamples, numStoredSamples, -numSamples);
        
        base.clear();
        base.swapWith (copying);
        numStoredSamples -= numSamples;
    }
    
    
private:
    
    int numStoredSamples = 0;  // even though no audio is stored, we need to keep an internal representation of the actual number of samples this buffer represents in time
    
    juce::MidiBuffer base;
    
    juce::MidiBuffer copying; // pre-allocated memory for copying left-over events back to the front of the base buffer after popping events...
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFIFO)
};


}  // namespace midi

} // namespace bav


