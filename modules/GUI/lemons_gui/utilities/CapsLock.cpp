#if JUCE_WINDOWS
#  include <Windows.h>
#elif JUCE_MAC
#  include <CoreGraphics/CoreGraphics.h>
#else
// not currently implemented for Linux
#endif

namespace lemons::gui::util
{

bool isCapsLockOn()
{
	bool result = false;

#if JUCE_WINDOWS
	result = (GetKeyState (VK_CAPITAL) & 0x0001) != 0;
#elif JUCE_MAC
	const auto flags = CGEventSourceFlagsState (kCGEventSourceStateHIDSystemState);
	result			 = (kCGEventFlagMaskAlphaShift & flags) != 0;
#else
	// not currently implemented for Linux
#endif

	return result;
}

}  // namespace lemons::gui::util
