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

#include "device/DeviceRotation.cpp"
#include "device/motion/DeviceMotion.cpp"

#if JUCE_IOS
#  include "device/motion/DeviceMotion_ios.mm"
#elif JUCE_ANDROID
#  include "device/motion/DeviceMotion_android.cpp"
#endif

#if JUCE_ANDROID
#  include "device/autolock/AutoLock_android.cpp"
#else
#  include "device/autolock/AutoLock.cpp"
#endif

#include "device/gps/GPS.cpp"

#if LEMONS_ENABLE_GPS_LOCATION_SERVICES && (JUCE_MAC || JUCE_IOS)
#  include "device/gps/GPS_apple.mm"
#elif LEMONS_ENABLE_GPS_LOCATION_SERVICES && JUCE_WINDOWS
#  include "device/gps/GPS_windows.cpp"
#elif LEMONS_ENABLE_GPS_LOCATION_SERVICES && JUCE_ANDROID
#  include "device/gps/GPS_android.cpp"
#else
#  include "device/gps/GPS_unsupported.cpp"
#endif

#include "app/DefaultWindow.cpp"

#if LEMONS_UNIT_TESTS
#  include "utilities/serializing_tests.cpp"
#endif
