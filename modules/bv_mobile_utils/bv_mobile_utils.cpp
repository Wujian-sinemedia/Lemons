
#include "bv_mobile_utils.h"

#if JUCE_IOS
#    include "iOS/iOSMotionManager.mm"


void AutoLock::setEnabled (bool shouldBeEnabled)
{
    juce::Desktop::getInstance()->setScreenSaverEnabled (shouldBeEnabled);
}


#elif JUCE_ANDROID
#    include "Android/AndroidAutoLock.cpp"


#else
/* dummy implementations of these classes that can be used on desktop (so you don't have to change your code) */

void AutoLock::setEnabled (bool) { }


MotionManager::MotionManager() { }

MotionManager::~MotionManager() { }

void MotionManager::start() { }

void MotionManager::stop() { }

bool MotionManager::isRunning() { return false; }


#endif
