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

#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_device
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_device
 description:        Classes for interacting with the device's physical state
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_core juce_gui_basics
 iOSFrameworks:      AppKit CoreLocation CoreMotion
 OSXFrameworks:      AppKit CoreLocation

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


//==============================================================================
/** Config: LEMONS_ENABLE_GPS_LOCATION_SERVICES

 Set this to 1 to enable GPS location services.
 */
#ifndef LEMONS_ENABLE_GPS_LOCATION_SERVICES
#	define LEMONS_ENABLE_GPS_LOCATION_SERVICES 0
#endif

//==============================================================================


#include "rotation/DeviceRotation.h"
#include "autolock/AutoLock.h"
#include "motion/DeviceMotion.h"
#include "gps/GPS.h"
