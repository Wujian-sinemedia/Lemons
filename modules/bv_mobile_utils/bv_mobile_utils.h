/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_mobile_utils
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_mobile_utils
 description:        JUCE module that provides some useful utilities for crossplatform iOS and Android development
 dependencies: juce_gui_basics
 iOSFrameworks: CoreMotion
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


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


struct AutoLock
{
    static void setEnabled (bool shouldBeEnabled);
};

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


/* Dummy Autolock struct that can be called on desktop platforms (does nothing) */
struct AutoLock
{
    static void setEnabled (bool) { }
};

#endif

}  // namespace bav
