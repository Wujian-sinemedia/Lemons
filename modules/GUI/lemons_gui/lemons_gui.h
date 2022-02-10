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

 ID:                 lemons_gui
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_gui
 description:        GUI utility classes and some basic components
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_core juce_gui_extra
 iOSFrameworks:      CoreGraphics
 OSXFrameworks:      CoreGraphics
 linuxLibs:          X11

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "unit_tests/gui_tests.h"

#include "utilities/serializing.h"
#include "utilities/colors.h"
#include "utilities/cursor.h"
#include "utilities/util.h"
#include "utilities/CapsLock.h"

#include "components/Buttons.h"
#include "components/SwappableComponent.h"
#include "components/sliders.h"
#include "components/Label.h"
#include "components/VersionViewer.h"
#include "components/popups/PopupComponent.h"
#include "components/popups/VersionBumpPrompt.h"
#include "components/popups/SaveToFilePrompt.h"

#include "app/DefaultWindow.h"
#include "app/GuiApp.h"
