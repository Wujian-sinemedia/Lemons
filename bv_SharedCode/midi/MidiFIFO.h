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
    }
    
    int numStoredEvents() { return base.getNumEvents(); }
    
    void pushEvents (const juce::MidiBuffer& source, const int numSamples)
    {
        const auto sourceStart = source.findNextSamplePosition (0);
        const auto sourceEnd   = source.findNextSamplePosition (numSamples);
        
        const int writingStartSample = base.isEmpty() ? 0 : base.getLastEventTime() + 1;
        
        std::for_each (sourceStart, sourceEnd,
                       [&] (const juce::MidiMessageMetadata& meta)
                       {
                           base.addEvent (meta.getMessage(),
                                          meta.samplePosition + writingStartSample);
                       } );
    }
    
    
    void popEvents (juce::MidiBuffer& output, const int numSamples)
    {
        output.clear();
        
        const auto readStart = base.findNextSamplePosition (0);
        const auto readEnd   = base.findNextSamplePosition (numSamples);
        
        std::for_each (readStart, readEnd,
                       [&] (const juce::MidiMessageMetadata& meta)
                       {
                           output.addEvent (meta.getMessage(), meta.samplePosition);
                       } );
        
        copying.clear();
        
        std::for_each (readEnd, base.cend(),
                       [&] (const juce::MidiMessageMetadata& meta)
                       {
                           copying.addEvent (meta.getMessage(),
                                             std::max(0, meta.samplePosition - numSamples));
                       } );
        
        base.clear();
        base.swapWith (copying);
    }
    
    
private:
    
    juce::MidiBuffer base;
    
    juce::MidiBuffer copying;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFIFO)
};

    
}  // namespace midi
    
} // namespace bav

