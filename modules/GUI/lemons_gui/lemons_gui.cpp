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

#include "lemons_gui.h"


#include "unit_tests/gui_tests.cpp"

#include "utilities/CapsLock.cpp"
#include "utilities/cursor.cpp"
#include "utilities/serializing.cpp"

#include "components/Buttons.cpp"
#include "components/Label.cpp"
#include "components/SwappableComponent.cpp"
#include "components/VersionViewer.cpp"
#include "components/popups/PopupComponent.cpp"
#include "components/popups/SaveToFilePrompt.cpp"
#include "components/popups/VersionBumpPrompt.cpp"
#include "components/sliders.cpp"

#include "app/DefaultWindow.cpp"

#if LEMONS_UNIT_TESTS
#	include "utilities/serializing_tests.cpp"
#endif
