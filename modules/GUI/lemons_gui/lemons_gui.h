#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_gui
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_gui
 description:        GUI utility classes and some basic components
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_core juce_gui_extra
 iOSFrameworks:      CoreMotion

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "utilities/serializing.h"
#include "utilities/colors.h"
#include "utilities/cursor.h"
#include "utilities/Fonts.h"

#include "components/PopupComponent.h"
#include "components/Buttons.h"
#include "components/Label.h"

#include "device/AutoLock.h"
#include "device/DeviceMotion.h"
#include "device/DeviceRotation.h"
