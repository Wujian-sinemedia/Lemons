macro (_bv_add_modules)

	juce_add_modules (${ARGV})

	list (APPEND modules ${ARGV})

	set (modules "${modules}" PARENT_SCOPE)

endmacro()

#

add_subdirectory (Common)

add_subdirectory (Audio)

add_subdirectory (GUI)

add_subdirectory (Plugins)

add_subdirectory (Apps)