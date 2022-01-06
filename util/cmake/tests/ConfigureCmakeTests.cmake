include_guard (GLOBAL)

if (NOT LEMONS_BUILD_TESTS)
	message (AUTHOR_WARNING "Cmake configuration is incorrect!")
endif()

enable_testing()

find_package (Python3 COMPONENTS Interpreter)

if (NOT Python3_Interpreter_FOUND)
	message (FATAL_ERROR "Python not found!")
endif()

add_test (NAME LemonsCmakeTests 
          COMMAND Python3::Interpreter "${CMAKE_CURRENT_LIST_DIR}/RunCmakeTests.py" "${Lemons_SOURCE_DIR}"
          WORKING_DIRECTORY "${Lemons_SOURCE_DIR}")
