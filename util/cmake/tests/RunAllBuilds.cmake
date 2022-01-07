include_guard (GLOBAL)

find_package (Python3 COMPONENTS Interpreter)

if (NOT TARGET Python3::Interpreter)
	message (FATAL_ERROR "Python not found!")
endif()


add_test (NAME Lemons.RunAllBuilds 
          COMMAND Python3::Interpreter "${CMAKE_CURRENT_LIST_DIR}/RunAllBuilds.py" "${Lemons_SOURCE_DIR}"
          WORKING_DIRECTORY "${Lemons_SOURCE_DIR}")
