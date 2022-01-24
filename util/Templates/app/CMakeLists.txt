cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (NewApp VERSION 0.0.1 LANGUAGES CXX)

include (LemonsAppModules)
include (BVBrandFlags)

set (sourceDir ${CMAKE_CURRENT_LIST_DIR}/Source)

lemons_add_juce_modules (DIR "${sourceDir}/modules" ALIAS_NAMESPACE
						 ${PROJECT_NAME})

juce_add_gui_app (
	${PROJECT_NAME}
	${BV_DEFAULT_BRAND_FLAGS}
	PRODUCT_NAME
	${PROJECT_NAME}
	VERSION
	${PROJECT_VERSION}
	DESCRIPTION
	""
	NEEDS_MIDI_INPUT
	TRUE
	NEEDS_MIDI_OUTPUT
	TRUE
	EDITOR_WANTS_KEYBOARD_FOCUS
	FALSE
	MICROPHONE_PERMISSION_ENABLED
	TRUE
	MICROPHONE_PERMISSION_TEXT
	""
	DOCUMENT_BROWSER_ENABLED
	TRUE
	BACKGROUND_AUDIO_ENABLED
	FALSE # for iOS
	NEEDS_CURL
	TRUE
	NEEDS_WEB_BROWSER
	TRUE
	ICON_BIG
	"" # path to icon file
	STATUS_BAR_HIDDEN
	TRUE # for iOS
	REQUIRES_FULL_SCREEN
	TRUE # for iOS
	IPAD_SCREEN_ORIENTATIONS
	UIInterfaceOrientationUnknown,
	UIInterfaceOrientationLandscapeLeft,
	UIInterfaceOrientationLandscapeRight
	TARGETED_DEVICE_FAMILY
	2 # target iPad only
	DOCUMENT_EXTENSIONS
	xml
	SEND_APPLE_EVENTS_PERMISSION_ENABLED
	FALSE)

lemons_configure_juce_app (TARGET ${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE ${sourceDir}/Main.cpp)

target_link_libraries (${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::new_app_gui)
