
#include "bv_mobile_utils.h"

#include "all_platforms/motion.cpp"
#include "all_platforms/rotation.cpp"

#if JUCE_IOS
#    include "iOS/iOSMotionManager.mm"

#elif JUCE_ANDROID
#    include "Android/androidMotion.cpp"
#    include "Android/AndroidAutoLock.cpp"

#endif


#if ! JUCE_ANDROID /* this impl works for iOS and desktop */
namespace bav
{
void AutoLock::setEnabled (bool shouldBeEnabled)
{
    juce::Desktop::getInstance().setScreenSaverEnabled (shouldBeEnabled);
}


}  // namespace bav

#endif /* if ! JUCE_ANDROID */
