include_guard (GLOBAL)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsDefaultPlatformSettings)

#

file (GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)

set (lemonsModulePaths "${CMAKE_CURRENT_LIST_DIR}")

foreach (child ${children})
	set (full_path "${CMAKE_CURRENT_LIST_DIR}/${child}")

	if (IS_DIRECTORY "${full_path}")
		file (REAL_PATH "${full_path}" _abs_path EXPAND_TILDE)
		list (APPEND lemonsModulePaths "${_abs_path}")
	endif()
endforeach()

list (APPEND CMAKE_MODULE_PATH "${lemonsModulePaths}")
list (APPEND LEMONS_CMAKE_MODULE_PATH "${lemonsModulePaths}")

list (REMOVE_DUPLICATES LEMONS_CMAKE_MODULE_PATH)

set (LEMONS_CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}" CACHE INTERNAL "")

#

set (CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard version")
set (CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")

set (CMAKE_CXX_VISIBILITY_PRESET hidden CACHE INTERNAL "")
set (CMAKE_VISIBILITY_INLINES_HIDDEN YES CACHE INTERNAL "")

