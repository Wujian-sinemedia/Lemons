/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_motion
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_motion
 description:        JUCE module to recieve data from motion sensors on mobile devices
 dependencies: juce_core
 iOSFrameworks: CoreMotion
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include "juce_core/juce_core.h"


namespace bav
{
#if JUCE_IOS || JUCE_ANDROID


class MotionManagerInterface
{
public:
    MotionManagerInterface() { }

    virtual ~MotionManagerInterface() = default;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual bool isRunning() = 0;

    double accelerationX = 0;
    double accelerationY = 0;
    double accelerationZ = 0;

    double gravityX = 0;
    double gravityY = 0;
    double gravityZ = 0;

    double rotationX = 0;
    double rotationY = 0;
    double rotationZ = 0;

    double attitudeX = 0;
    double attitudeY = 0;
    double attitudeZ = 0;
};


#    if JUCE_IOS
#        include "iOS/iosMotion.h"
#    elif JUCE_ANDROID
#        include "Android/androidMotion.h"
#    endif

#else /* if JUCE_IOS || JUCE_ANDROID */

/* This dummy Motionmanager class can be instantiated on non-mobile devices */
class MotionManager
{
public:
    MotionManager()  = default;
    ~MotionManager() = default;

    void start() { }
    void stop() { }

    bool isRunning() { return false; }

    double accelerationX = 0;
    double accelerationY = 0;
    double accelerationZ = 0;

    double gravityX = 0;
    double gravityY = 0;
    double gravityZ = 0;

    double rotationX = 0;
    double rotationY = 0;
    double rotationZ = 0;

    double attitudeX = 0;
    double attitudeY = 0;
    double attitudeZ = 0;
};

#endif

}  // namespace bav
