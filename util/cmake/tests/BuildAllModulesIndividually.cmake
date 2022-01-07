include_guard (GLOBAL)

if (NOT LEMONS_BUILD_TESTS)
	message (AUTHOR_WARNING "Cmake configuration is incorrect!")
endif()


# For each juce module, create a dummy executable linked only to that module, to test that each module can be built individually

add_custom_target (LemonsModuleIndividualTests
				   COMMENT "Building juce module independancy tests...")

include (AllLemonsModules)

foreach (moduleName ${lemons_all_modules})

	if (NOT TARGET "Lemons::${moduleName}")
		message (AUTHOR_WARNING "Juce module ${moduleName} not found!")
		continue()
	endif()

	set (moduleTestName "${moduleName}_test")

	if (TARGET "${moduleTestName}")
		continue()
	endif()

	set (generated_file "${moduleName}_test.cpp")

	configure_file ("${CMAKE_CURRENT_LIST_DIR}/ModuleTestMain.cpp" "${generated_file}"
					@ONLY
					NEWLINE_STYLE UNIX)

	juce_add_console_app (${moduleTestName})

	lemons_configure_juce_target (TARGET ${moduleTestName})

	target_sources (${moduleTestName} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${generated_file}")

	target_link_libraries (${moduleTestName} PRIVATE Lemons::${moduleName})

	add_dependencies (LemonsModuleIndividualTests ${moduleTestName})

endforeach()


add_test (NAME Lemons.BuildEachJuceModuleIndividually
		  COMMAND "${CMAKE_COMMAND}" --build "${Lemons_BINARY_DIR}" --target LemonsModuleIndividualTests
		  WORKING_DIRECTORY "${Lemons_SOURCE_DIR}")

