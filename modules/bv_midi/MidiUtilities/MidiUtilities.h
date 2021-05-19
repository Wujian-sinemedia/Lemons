
namespace bav::midi
{
/*  Returns the pitch class number for a given string.
 If the string is not in the required format i.e. A#4 etc. this
 will return -1.
 */
inline int pitchClassFromString (const juce::String& pitchClassName) noexcept
{
    int pitchClass = -1;
    const auto numChars = pitchClassName.length();
    
    if (numChars > 0)
    {
        const auto base = pitchClassName.toLowerCase()[0];
        
        switch (base)
        {
            case 'c':   pitchClass = 0;     break;
            case 'd':   pitchClass = 2;     break;
            case 'e':   pitchClass = 4;     break;
            case 'f':   pitchClass = 5;     break;
            case 'g':   pitchClass = 7;     break;
            case 'a':   pitchClass = 9;     break;
            case 'b':   pitchClass = 11;    break;
            default:    pitchClass = -1;    break;
        }
    }
    
    if (numChars > 1)
    {
        const auto sharpOrFlat = pitchClassName[1];
        
        switch (sharpOrFlat)
        {
            case '#':   ++pitchClass;       break;
            case 'b':   --pitchClass;       break;
            default:                        break;
        }
        
        if (sharpOrFlat == getSharpSymbol())
            ++pitchClass;
        else if (sharpOrFlat == getFlatSymbol())
            --pitchClass;
        
        pitchClass %= 12;
    }
    
    return pitchClass;
}


// returns true if the given midinote is a black key
template<typename noteType>
inline bool isMidiNoteBlackKey (noteType midipitch)
{
    jassert (midipitch >= noteType(0) && midipitch <= noteType(127));
    
    switch (midipitch % 12)
    {
        case 1:  return true;
        case 3:  return true;
        case 6:  return true;
        case 8:  return true;
        case 10: return true;
        default: return false;
    }
}


// returns true if the two pitches are the same pitch class, regardless of octave
template<typename noteType1, typename noteType2>
inline bool areNotesSamePitchClass (const noteType1 pitch1, const noteType2 pitch2)
{
    jassert (pitch1 >= noteType1(0) && pitch2 >= noteType2(0) && pitch1 <= noteType1(127) && pitch2 <= noteType2(127));
    return (pitch1 % 12 == pitch2 % 12);
}


// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
inline void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer, juce::MidiBuffer& destBuffer,
                                   const int startSampleOfInput,
                                   const int startSampleOfOutput,
                                   const int numSamples)
{
    destBuffer.clear (startSampleOfOutput, numSamples);
    destBuffer.addEvents (readingBuffer, startSampleOfInput, numSamples, startSampleOfOutput - startSampleOfInput);
}


} // namespace


