include_guard (GLOBAL)

macro (_lemons_add_cmake_module_dir path)
	file (REAL_PATH "${path}" _abs_path EXPAND_TILDE)
	list (APPEND CMAKE_MODULE_PATH "${_abs_path}")
endmacro()

_lemons_add_cmake_module_dir ("${CMAKE_CURRENT_LIST_DIR}/modules")
_lemons_add_cmake_module_dir ("${CMAKE_CURRENT_LIST_DIR}/util/cmake/scripts")

foreach (directory "util/cmake/modules" "modules")
	set (directory "${CMAKE_CURRENT_LIST_DIR}/${directory}")
	file (GLOB children RELATIVE ${directory} ${directory}/*)

	foreach (child ${children})
		set (full_path "${directory}/${child}")

		if (IS_DIRECTORY "${full_path}")
			_lemons_add_cmake_module_dir ("${full_path}")
		endif()
	endforeach()
endforeach()

# force CMake module paths to propogate outwards to any containing projects...
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "")

set (LEMONS_CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "")
