include_guard (GLOBAL)

if (NOT LEMONS_BUILD_TESTS)
	message (AUTHOR_WARNING "Cmake configuration is incorrect!")
endif()


# For each juce module, create a dummy executable linked only to that module, to test that each module can be built individually

add_custom_target (LemonsModuleIndividualTests
				   COMMENT "Building juce module independancy tests...")

include (AllLemonsModules)

get_target_property (lemons_module_categories Lemons::AllLemonsModules ModuleCategoryNames)

if (NOT lemons_module_categories)
	message (AUTHOR_WARNING "Error retrieving juce module category names!")
	return()
endif()

foreach (module_category ${lemons_module_categories})

	if (NOT TARGET Lemons::${module_category})
		message ("Module category target ${module_category} not found!")
		continue()
	endif()

	get_target_property (module_names Lemons::${module_category} OriginalModuleNames)

	if (NOT module_names)
		message (AUTHOR_WARNING "Error retrieving original juce module names from category ${module_names}!")
		return()
	endif()

	foreach (moduleName ${module_names})

		if (NOT TARGET "Lemons::${moduleName}")
			message (AUTHOR_WARNING "Juce module ${moduleName} not found!")
			continue()
		endif()

		set (moduleTestName "${moduleName}_test")

		if (TARGET "${moduleTestName}")
			message (AUTHOR_WARNING "Duplicate juce module test ${moduleTestName} found!")
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

endforeach()


add_dependencies (LemonsTests LemonsModuleIndividualTests)

