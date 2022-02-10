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

#include "lemons_device.h"


#include "rotation/DeviceRotation.cpp"
#include "motion/DeviceMotion.cpp"

#if JUCE_IOS
#	include "motion/DeviceMotion_ios.mm"
#elif JUCE_ANDROID
#	include "motion/DeviceMotion_android.cpp"
#endif

#if JUCE_ANDROID
#	include "autolock/AutoLock_android.cpp"
#else
#	include "autolock/AutoLock.cpp"
#endif

#include "gps/GPS.cpp"

#if LEMONS_ENABLE_GPS_LOCATION_SERVICES && (JUCE_MAC || JUCE_IOS)
#	include "gps/GPS_apple.mm"
#elif LEMONS_ENABLE_GPS_LOCATION_SERVICES && JUCE_WINDOWS
#	include "gps/GPS_windows.cpp"
#elif LEMONS_ENABLE_GPS_LOCATION_SERVICES && JUCE_ANDROID
#	include "gps/GPS_android.cpp"
#else
#	include "gps/GPS_unsupported.cpp"
#endif
