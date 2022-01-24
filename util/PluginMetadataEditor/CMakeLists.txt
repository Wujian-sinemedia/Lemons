include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (PluginMetadataEditor VERSION 0.1.0 LANGUAGES CXX)

#

if (NOT DEFINED CPM_Lemons_SOURCE)
	set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif ()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
	set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif ()

#

include ("${CPM_Lemons_SOURCE}/scripts/AddLemons.cmake")

include (AllLemonsModules)
include (BVBrandFlags)

#

juce_add_module ("${CMAKE_CURRENT_LIST_DIR}/metadata_editor_gui")

juce_add_gui_app (
	${PROJECT_NAME}
	${BV_DEFAULT_BRAND_FLAGS}
	PRODUCT_NAME
	${PROJECT_NAME}
	VERSION
	${PROJECT_VERSION}
	DOCUMENT_EXTENSIONS
	xml
	json
	SEND_APPLE_EVENTS_PERMISSION_ENABLED
	FALSE
	IS_SYNTH
	FALSE
	MICROPHONE_PERMISSION_ENABLED
	FALSE
	DESCRIPTION
	"GUI editor for plugin metadata"
	DOCUMENT_BROWSER_ENABLED
	TRUE
	NEEDS_CURL
	FALSE
	NEEDS_WEB_BROWSER
	FALSE)

lemons_configure_juce_app (TARGET ${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_link_libraries (${PROJECT_NAME} PRIVATE metadata_editor_gui
											   Lemons::AllLemonsModules)
