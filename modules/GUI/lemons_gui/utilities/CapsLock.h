#pragma once

namespace lemons::gui::util
{

/** Returns true if the caps lock key is currently activated.
	This is not currently implemented for Linux; on Linux systems this function will always return false.
 */
[[nodiscard]] bool isCapsLockOn();

}  // namespace lemons::gui::util
