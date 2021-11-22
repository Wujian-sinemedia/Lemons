#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_mobile_utils
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_mobile_utils
 description:        JUCE module that provides some useful utilities for crossplatform iOS and Android development
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       juce_gui_basics
 iOSFrameworks:      CoreMotion

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#include "all_platforms/motion.h"
#include "all_platforms/rotation.h"

#if JUCE_IOS
#  include "iOS/iosMotion.h"
#elif JUCE_ANDROID
#  include "Android/androidMotion.h"
#else
#  include "dummy_desktop_versions/DummyMotionManager.h"
#endif


namespace lemons
{
// Control the auto-locking of the screen on mobile devices. On desktop, this disables the computer's screensaver.
struct AutoLock
{
	static void setEnabled (bool shouldBeEnabled);
};


struct Platform
{
	static constexpr bool isMobile()
	{
#if JUCE_IOS || JUCE_ANDROID
		return true;
#endif
		return false;
	}
};


}  // namespace lemons
