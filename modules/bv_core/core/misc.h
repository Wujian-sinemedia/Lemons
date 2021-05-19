
namespace bav
{


/* A less verbose way of checking if this is the MessageThread.
 */
static inline bool thisIsTheMessageThread() noexcept
{
    return juce::MessageManager::getInstance()->isThisTheMessageThread();
}



/* Returns a unicode sharp symbol.
 */
static inline const juce::juce_wchar getSharpSymbol() noexcept   {   return *juce::CharPointer_UTF8 ("\xe2\x99\xaf");  }

/* Returns a unicode flat symbol.
 */
static inline const juce::juce_wchar getFlatSymbol() noexcept    {   return *juce::CharPointer_UTF8 ("\xe2\x99\xad");  }

/* Returns a unicode natural symbol.
 */
static inline const juce::juce_wchar getNaturalSymbol() noexcept {   return *juce::CharPointer_UTF8 ("\xe2\x99\xae");  }


static inline int midiPanStringToInt (const juce::String& string)
{
    if (string.endsWithIgnoreCase (TRANS ("C")))
        return 64;
    
    if (string.endsWithIgnoreCase (TRANS ("R")))
        return juce::jmap (string.dropLastCharacters(1).getIntValue(), 1, 50, 65, 127);
    
    if (string.endsWithIgnoreCase (TRANS ("L")))
        return juce::jmap (string.dropLastCharacters(1).getIntValue(), 1, 50, 63, 0);
    
    return string.getIntValue();
}


static inline juce::String midiPanIntToString (const int midiPan)
{
    if (midiPan == 64)
        return juce::String (TRANS ("C"));
    
    if (midiPan > 64)
    {
        const auto amtRight = juce::jmap (midiPan, 65, 127, 1, 50);
        return juce::String (amtRight) + TRANS ("R");
    }
    
    const auto amtLeft = juce::jmap (midiPan, 63, 0, 1, 50);
    return juce::String (amtLeft) + TRANS ("L");
}


// converts a midi pitch to a string representation, eg note 69 = A4
inline juce::String pitchToString (const int midiNoteNumber, const bool asSharps = true)
{
    jassert (midiNoteNumber >= 0 && midiNoteNumber <= 127);
    
    const auto pitchClass = midiNoteNumber % 12;
    
    constexpr auto inv12 = 1.0f / 12.0f;
    const auto octave = juce::roundToInt (float(midiNoteNumber) * inv12) - 1;
    
    static const char* const sharpNoteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    static const char* const flatNoteNames[]  = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };
    auto noteName = asSharps ? juce::String (sharpNoteNames[pitchClass]) : juce::String (flatNoteNames[pitchClass]);
    
    return noteName + juce::String (octave);
}


// converts a pitch class string to an integer representation, eg A4 = note 69
inline int stringToPitch (const juce::String& string)
{
    const auto octaveName = string.retainCharacters ("0123456789");
    
    const auto pitchClassTokens = juce::String("abcdefg#") + getSharpSymbol() + getFlatSymbol() + getNaturalSymbol();
    const auto pitchClassName = string.toLowerCase().retainCharacters (pitchClassTokens);
    
    const auto octave = octaveName.getIntValue();
    const auto pitchClass = pitchClassFromString (pitchClassName);
    jassert (pitchClass > -1);
    
    return (octave * 12) + pitchClass;
}



}  // namespace bav
