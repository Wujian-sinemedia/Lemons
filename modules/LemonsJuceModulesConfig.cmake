include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

#

find_package (LemonsCmakeModules REQUIRED CONFIG)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsDefaultProjectSettings)

#

file (GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR}
	  ${CMAKE_CURRENT_LIST_DIR}/*)

list (REMOVE_ITEM children Builds)

set (lemonsModulePaths "${CMAKE_CURRENT_LIST_DIR}")

foreach (child ${children})
	set (full_path "${CMAKE_CURRENT_LIST_DIR}/${child}")

	if (IS_DIRECTORY "${full_path}")
		file (REAL_PATH "${full_path}" _abs_path EXPAND_TILDE)
		list (APPEND lemonsModulePaths "${_abs_path}")
	endif ()
endforeach ()

list (APPEND CMAKE_MODULE_PATH "${lemonsModulePaths}")
list (APPEND LEMONS_CMAKE_MODULE_PATH "${lemonsModulePaths}")

list (REMOVE_DUPLICATES LEMONS_CMAKE_MODULE_PATH)
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

set (LEMONS_CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}" CACHE INTERNAL "")
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "")
