include_guard (GLOBAL)

include (LemonsFileUtils)

set (TEST_FILE_PATHS "")

foreach (module_path ${CMAKE_MODULE_PATH})

	set (test_files_dir "${module_path}/tests")

	if (NOT IS_DIRECTORY "${test_files_dir}")
		continue()
	endif()

	lemons_subdir_list (FILES DIR ${test_files_dir} RESULT test_files)

	foreach (test_file ${test_files})
		set (filepath "${test_files_dir}/${test_file}")

		if (NOT ${filepath} IN_LIST TEST_FILE_PATHS)
			list (APPEND TEST_FILE_PATHS ${filepath})
		endif()
	endforeach()
endforeach()

if (NOT TEST_FILE_PATHS)
	message (AUTHOR_WARNING "No CMake test files found!")
	return()
endif()

configure_file ("${CMAKE_CURRENT_LIST_DIR}/RunCmakeUnitTests.cmake" "RunCmakeUnitTests.cmake"
			    @ONLY
			    NEWLINE_STYLE UNIX)

add_test (NAME CmakeUnitTests
		  COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/RunCmakeUnitTests.cmake")
