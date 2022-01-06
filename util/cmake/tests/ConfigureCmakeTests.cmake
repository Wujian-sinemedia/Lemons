include_guard (GLOBAL)

if (NOT LEMONS_BUILD_TESTS)
	message (AUTHOR_WARNING "Cmake configuration is incorrect!")
endif()


find_package (Python3 COMPONENTS Interpreter)

if (NOT Python3_Interpreter_FOUND)
	message (FATAL_ERROR "Python not found!")
endif()


enable_testing()

add_test (NAME RunAllBuilds 
          COMMAND Python3::Interpreter "${CMAKE_CURRENT_LIST_DIR}/RunAllBuilds.py" "${Lemons_SOURCE_DIR}"
          WORKING_DIRECTORY "${Lemons_SOURCE_DIR}")
