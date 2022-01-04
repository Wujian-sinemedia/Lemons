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



#include "unit_tests/gui_tests.h"

#include "utilities/serializing.h"
#include "utilities/colors.h"
#include "utilities/cursor.h"
#include "utilities/util.h"

#include "components/Buttons.h"
#include "components/SwappableComponent.h"
#include "components/sliders.h"
#include "components/Label.h"
#include "components/VersionViewer.h"
#include "components/popups/PopupComponent.h"
#include "components/popups/VersionBumpPrompt.h"
#include "components/popups/SaveToFilePrompt.h"

#include "device/AutoLock.h"
#include "device/DeviceMotion.h"
#include "device/DeviceRotation.h"

#include "app/DefaultWindow.h"
#include "app/GuiApp.h"
