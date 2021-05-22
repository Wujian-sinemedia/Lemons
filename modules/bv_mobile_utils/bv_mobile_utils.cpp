
#include "bv_mobile_utils.h"

#if JUCE_IOS
#    include "iOS/iOSMotionManager.mm"


void AutoLock::setEnabled (bool shouldBeEnabled)
{
    juce::Desktop::getInstance()->setScreenSaverEnabled (shouldBeEnabled);
}


#elif JUCE_ANDROID

#    include "Android/AndroidAutoLock.cpp"

#endif
