include_guard (GLOBAL)

include (AllLemonsModules)

install (TARGETS AllLemonsModules EXPORT LemonsTargets
		 LIBRARY DESTINATION lib
		 ARCHIVE DESTINATION lib
		 RUNTIME DESTINATION bin
		 INCLUDES DESTINATION include)

install (EXPORT LemonsTargets 
		 FILE LemonsTargets.cmake
		 NAMESPACE Lemons::
		 DESTINATION lib/cmake/Lemons)

include (CMakePackageConfigHelpers)

write_basic_package_version_file ("LemonsConfigVersion.cmake"
								  VERSION ${Lemons_VERSION}
								  COMPATIBILITY SameMajorVersion)

install (FILES "LemonsConfigVersion.cmake"
		 DESTINATION lib/cmake/Lemons)
