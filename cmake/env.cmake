if (DEFINED ENV{BV_IGNORE_CCACHE})
	option (IgnoreCcache "Ignore ccache" $ENV{BV_IGNORE_CCACHE})
endif()

if (DEFINED ENV{BV_IGNORE_CLANGTIDY})
	option (IgnoreClangTidy "Ignore clang-tidy" $ENV{BV_IGNORE_CLANGTIDY})
endif()

if (DEFINED ENV{BV_IGNORE_CPPCHECK})
	option (IgnoreCppCheck "Ignore CppCheck" $ENV{BV_IGNORE_CPPCHECK})
endif()

if (DEFINED ENV{BV_USE_LV2_JUCE})
	option (BV_USE_LV2_JUCE "Use the fork of JUCE that enables compiling to LV2 on Linux" $ENV{BV_USE_LV2_JUCE})
endif()

if (DEFINED ENV{BV_COPY_TO_DEPLOY_FOLDER})
	option (BV_COPY_TO_DEPLOY_FOLDER "Copies each product's install components to /Builds/deploy/<ProductName>" $ENV{BV_COPY_TO_DEPLOY_FOLDER})
endif()

if (DEFINED ENV{BV_SANITIZERS_TO_ENABLE})
	set (BV_SANITIZERS_TO_ENABLE $ENV{BV_SANITIZERS_TO_ENABLE})
endif()

if (DEFINED ENV{BV_CPACK_GENERATOR})
	set (CPACK_GENERATOR $ENV{BV_CPACK_GENERATOR})
endif()
