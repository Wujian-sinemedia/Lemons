/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_mobile_utils
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_mobile_utils
 description:        JUCE module that provides some useful utilities for crossplatform iOS and Android development
 dependencies:       juce_gui_basics
 iOSFrameworks:      CoreMotion
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace bav
{
// to-do: rotation lock

/*=====================================================================================================
 Classes dealing with screen rotation
 =====================================================================================================*/

/* This struct listens for changes to a mobile device's orientation and recieves a callback when it changes.
 This class does nothing on desktop, and the callback will never be called. */

struct DeviceRotationListener : private juce::Timer
{
    DeviceRotationListener (std::function< void() > callbackToUse);
    virtual ~DeviceRotationListener();

    void timerCallback() override final;

private:
    std::function< void() >           callback;
    juce::Desktop::DisplayOrientation prevOrientation;
};


/* This struct allows you to lock the screen from rotating, or easily set landscape-only or vertical-only rotations. */

struct DeviceRotation
{
    static void disableRotation();

    static void enableOnlyVerticalRotations();
    static void enableOnlyLandscapeRotations();
};


/*=====================================================================================================
 Control the auto-locking of the screen on mobile devices. On desktop, this disables the computer's screensaver.
 =====================================================================================================*/

struct AutoLock
{
    static void setEnabled (bool shouldBeEnabled);
};


/*=====================================================================================================
 Classes dealing with motion & movement sensors
=====================================================================================================*/

struct MotionListener
{
    virtual ~MotionListener() = default;

    virtual void accelerationChanged (double, double, double) { }
    virtual void gravityChanged (double, double, double) { }
    virtual void rotationChanged (double, double, double) { }
    virtual void attitudeChanged (double, double, double) { }
};


class MotionManagerInterface
{
public:
    struct Coords
    {
        double x = 0;
        double y = 0;
        double z = 0;
    };

    MotionManagerInterface()          = default;
    virtual ~MotionManagerInterface() = default;

    virtual void start()     = 0;
    virtual void stop()      = 0;
    virtual bool isRunning() = 0;

    Coords getAcceleration() const { return acceleration; }
    Coords getGravity() const { return gravity; }
    Coords getRotation() const { return rotation; }
    Coords getAttitude() const { return attitude; }

    void addListener (MotionListener* l) { listeners.add (l); }
    void removeListener (MotionListener* l) { listeners.remove (l); }

protected:
    void accelerationChanged (double x, double y, double z);
    void gravityChanged (double x, double y, double z);
    void rotationChanged (double x, double y, double z);
    void attitudeChanged (double x, double y, double z);

    Coords acceleration;
    Coords gravity;
    Coords rotation;
    Coords attitude;

    juce::ListenerList< MotionListener > listeners;
};


}  // namespace bav

/*=====================================================================================================
 =====================================================================================================*/


#if JUCE_IOS
#    include "iOS/iosMotion.h"

#elif JUCE_ANDROID
#    include "Android/androidMotion.h"

#else
#    include "dummy_desktop_versions/DummyMotionManager.h"

#endif
