
#pragma once


namespace bav
{


static inline bool thisIsTheMessageThread() noexcept
{
    return juce::MessageManager::getInstance()->isThisTheMessageThread();
}


/* Returns a unicode sharp symbol.
 */
static inline const juce::juce_wchar getSharpSymbol() noexcept
{
    return *juce::CharPointer_UTF8 ("\xe2\x99\xaf");
}

/* Returns a unicode flat symbol.
 */
static inline const juce::juce_wchar getFlatSymbol() noexcept
{
    return *juce::CharPointer_UTF8 ("\xe2\x99\xad");
}

/* Returns a unicode natural symbol.
 */
static inline const juce::juce_wchar getNaturalSymbol() noexcept
{
    return *juce::CharPointer_UTF8 ("\xe2\x99\xae");
}


extern int midiPanStringToInt (const juce::String& string);


extern juce::String midiPanIntToString (const int midiPan);


// converts a midi pitch to a string representation, eg note 69 = A4
extern juce::String pitchToString (const int  midiNoteNumber,
                            const bool asSharps = true);


/*  Returns the pitch class number for a given string.
 If the string is not in the required format i.e. A#4 etc. this
 will return -1.
 */
extern int pitchClassFromString (const juce::String& pitchClassName) noexcept;


// converts a pitch class string to an integer representation, eg A4 = note 69
extern int stringToPitch (const juce::String& string);


}  // namespace bav
