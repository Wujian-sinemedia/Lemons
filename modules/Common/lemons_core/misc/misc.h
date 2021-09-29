
#pragma once


namespace lemons
{
bool thisIsTheMessageThread() noexcept;

template < typename T >
inline void atomic_swap (std::atomic< T >& atomic, const T& newValue)
{
    const auto before = atomic.load();

    while (! atomic.compare_exchange_weak (before, newValue))
    {
    }
}

/* Returns a unicode sharp symbol.
 */
const juce::juce_wchar getSharpSymbol() noexcept;

/* Returns a unicode flat symbol.
 */
const juce::juce_wchar getFlatSymbol() noexcept;

/* Returns a unicode natural symbol.
 */
const juce::juce_wchar getNaturalSymbol() noexcept;


int midiPanStringToInt (const String& string);

String midiPanIntToString (const int midiPan);


// converts a midi pitch to a string representation, eg note 69 = A4
String pitchToString (const int  midiNoteNumber,
                      const bool asSharps = true);


/*  Returns the pitch class number for a given string.
 If the string is not in the required format i.e. A#4 etc. this
 will return -1.
 */
int pitchClassFromString (const String& pitchClassName) noexcept;


// converts a pitch class string to an integer representation, eg A4 = note 69
int stringToPitch (const String& string);


struct SystemInitializer
{
    SystemInitializer();

    virtual ~SystemInitializer() = default;
};

}  // namespace bav
