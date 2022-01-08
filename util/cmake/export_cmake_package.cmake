include_guard (GLOBAL)

include (AllLemonsModules)

include (GNUInstallDirs)

install (TARGETS Lemons::AllLemonsModules EXPORT LemonsTargets
		 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		 ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		 INCLUDES DESTINATION include)

install (EXPORT LemonsTargets 
		 FILE LemonsTargets.cmake
		 NAMESPACE Lemons::
		 DESTINATION lib/cmake/Lemons)

include (CMakePackageConfigHelpers)

write_basic_package_version_file ("LemonsConfigVersion.cmake"
								  VERSION ${Lemons_VERSION}
								  COMPATIBILITY SameMajorVersion)

configure_file ("${CPM_Lemons_SOURCE}/AddLemons.cmake" "LemonsConfig.cmake"
			    COPYONLY
			    NEWLINE_STYLE UNIX)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/LemonsConfig.cmake" "LemonsConfigVersion.cmake"
		 DESTINATION lib/cmake/Lemons)
