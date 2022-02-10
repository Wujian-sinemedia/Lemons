/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#if JUCE_WINDOWS
#	include <Windows.h>
#elif JUCE_MAC
#	include <CoreGraphics/CoreGraphics.h>
#else
#	include <X11/XKBlib.h>
#endif

namespace lemons::gui::util
{

bool isCapsLockOn()
{
#if JUCE_WINDOWS

	return (GetKeyState (VK_CAPITAL) & 0x0001) != 0;

#elif JUCE_MAC

	const auto flags = CGEventSourceFlagsState (kCGEventSourceStateHIDSystemState);
	return (kCGEventFlagMaskAlphaShift & flags) != 0;

#else

	if (auto* display = XOpenDisplay (getenv ("DISPLAY")))
	{
		unsigned state;

		if (XkbGetIndicatorState (display, XkbUseCoreKbd, &state) != Success)
			return false;

		return state & 1;
	}

#endif

	return false;
}

}  // namespace lemons::gui::util
