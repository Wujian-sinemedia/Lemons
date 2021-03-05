
namespace bav
{
    
namespace midi
{
    
    bool isMidiNoteBlackKey (const int midipitch)
    {
        jassert (midipitch >= 0 && midipitch <= 127);
        
        switch (midipitch % 12)
        {
            case 1: return true;
            case 3: return true;
            case 6: return true;
            case 8: return true;
            case 10: return true;
            default: return false;
        }
    }
    
    
    bool areNotesSamePitchClass (const int pitch1, const int pitch2)
    {
        jassert (pitch1 >= 0 && pitch2 >= 0 && pitch1 <= 127 && pitch2 <= 127);
        return (pitch1 % 12 == pitch2 % 12);
    }
    
    
    void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer, juce::MidiBuffer& destBuffer,
                                const int startSampleOfInput,
                                const int startSampleOfOutput,
                                const int numSamples)
    {
        destBuffer.clear (startSampleOfOutput, numSamples);
        
        auto midiIterator = readingBuffer.findNextSamplePosition(startSampleOfInput);
        
        if (midiIterator == readingBuffer.cend())
            return;
        
        const auto midiEnd = readingBuffer.findNextSamplePosition(startSampleOfInput + numSamples);
        
        if (midiIterator == midiEnd)
            return;
        
        const int sampleOffset = startSampleOfOutput - startSampleOfInput;
        
        std::for_each (midiIterator, midiEnd,
                       [&] (const juce::MidiMessageMetadata& meta)
                       {
                           destBuffer.addEvent (meta.getMessage(),
                                                std::max (0, meta.samplePosition + sampleOffset));
                       } );
    }

    
    
}  // namespace midi
    
}  // namespace bav
