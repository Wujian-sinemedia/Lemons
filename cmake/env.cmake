# set up CPM source cache in a git-ignored location (makes successive build configs MUCH faster!)
# if you wish to refresh the cache, I suggest manually deleting the cached files from the Cache dir. They will be freshly re-downloaded when the next CMake configuration is run.
if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
	if (LEMONS_WORKSPACE_BUILD)
		set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../../Cache)
	else()
		set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
	endif()
endif()


# optional list of sanitizers to enable
if (ENV{LEMONS_SANITIZERS_TO_ENABLE})
	set (LEMONS_SANITIZERS_TO_ENABLE $ENV{LEMONS_SANITIZERS_TO_ENABLE})
endif()


###  options  ###

if (DEFINED ENV{LEMONS_USE_LV2_JUCE})
	option (LEMONS_USE_LV2_JUCE "Use the fork of JUCE that enables compiling to LV2 on Linux" $ENV{LEMONS_USE_LV2_JUCE})
endif()

if (DEFINED ENV{LEMONS_COPY_TO_DEPLOY_FOLDER})
	option (LEMONS_COPY_TO_DEPLOY_FOLDER "Copies each product's install components to /Builds/deploy/<ProductName>" $ENV{LEMONS_COPY_TO_DEPLOY_FOLDER})
endif()

if (DEFINED ENV{LEMONS_CPACK_GENERATOR})
	set (CPACK_GENERATOR $ENV{LEMONS_CPACK_GENERATOR})
endif()
