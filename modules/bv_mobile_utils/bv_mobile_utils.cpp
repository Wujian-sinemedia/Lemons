
#include "bv_mobile_utils.h"


namespace bav
{

/*=====================================================================================================
 Implementations of the DeviceRotationListener methods -- for all platforms
 =====================================================================================================*/

DeviceRotationListener::DeviceRotationListener (std::function<void()> callbackToUse)
: callback (std::move (callbackToUse))
{
#if JUCE_IOS || JUCE_ANDROID
    Timer::startTimerHz (60);
#endif
}

DeviceRotationListener::~DeviceRotationListener()
{
#if JUCE_IOS || JUCE_ANDROID
    Timer::stopTimer();
#endif
}

void DeviceRotationListener::timerCallback()
{
    const auto orientation = juce::Desktop::getInstance().getCurrentOrientation();
    
    if (orientation != prevOrientation)
    {
        callback();
        prevOrientation = orientation;
    }
}


/*=====================================================================================================
 Implementations of the DeviceRotation methods -- for all platforms
 =====================================================================================================*/

void DeviceRotation::disableRotation()
{
    auto& desktop = juce::Desktop::getInstance();
    desktop.setOrientationsEnabled (desktop.getCurrentOrientation());
}

void DeviceRotation::enableOnlyVerticalRotations()
{
    using Desktop = juce::Desktop;
    Desktop::getInstance().setOrientationsEnabled (Desktop::upright | Desktop::upsideDown);
}

void DeviceRotation::enableOnlyLandscapeRotations()
{
    using Desktop = juce::Desktop;
    Desktop::getInstance().setOrientationsEnabled (Desktop::rotatedClockwise | Desktop::rotatedAntiClockwise);
}


/*=====================================================================================================
 Implementations of the MotionManagerInterface methods -- for all platforms
 =====================================================================================================*/

void MotionManagerInterface::accelerationChanged (double x, double y, double z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
    
    listeners.call ([this](MotionListener& l){ l.accelerationChanged (acceleration.x, acceleration.y, acceleration.z); });
}

void MotionManagerInterface::gravityChanged (double x, double y, double z)
{
    gravity.x = x;
    gravity.y = y;
    gravity.z = z;
    
    listeners.call ([this](MotionListener& l){ l.gravityChanged (gravity.x, gravity.y, gravity.z); });
}

void MotionManagerInterface::rotationChanged (double x, double y, double z)
{
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
    
    listeners.call ([this](MotionListener& l){ l.rotationChanged (rotation.x, rotation.y, rotation.z); });
}

void MotionManagerInterface::attitudeChanged (double x, double y, double z)
{
    attitude.x = x;
    attitude.y = y;
    attitude.z = z;
    
    listeners.call ([this](MotionListener& l){ l.attitudeChanged (attitude.x, attitude.y, attitude.z); });
}


/*=====================================================================================================
 =====================================================================================================*/

#if JUCE_IOS
#    include "iOS/iOSMotionManager.mm"

void AutoLock::setEnabled (bool shouldBeEnabled)
{
    juce::Desktop::getInstance().setScreenSaverEnabled (shouldBeEnabled);
}

/*=====================================================================================================
 =====================================================================================================*/

#elif JUCE_ANDROID
#    include "Android/androidMotion.cpp"
#    include "Android/AndroidAutoLock.cpp"

/*=====================================================================================================
 =====================================================================================================*/

#else


void AutoLock::setEnabled (bool shouldBeEnabled)
{
    juce::Desktop::getInstance().setScreenSaverEnabled (shouldBeEnabled);
}


}  // namespace bav

#endif
