
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



}  // namespace bav
