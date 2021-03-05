/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */


namespace bav

{

namespace midi
{


class MidiFIFO  :    public juce::MidiBuffer
{
    
public:
    
    MidiFIFO() { }
    
    ~MidiFIFO() { }
    
    void pushEvents (const juce::MidiBuffer& source, const int numSamples);
    
    
    void popEvents (juce::MidiBuffer& output, const int numSamples);
    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFIFO)
};

    
}  // namespace midi
    
} // namespace bav

