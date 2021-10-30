option (LEMONS_IOS_SIMULATOR "Compile for an iOS simulator, rather than a real device" ON)

#

set (CMAKE_CROSSCOMPILING TRUE CACHE INTERNAL "")

set (CMAKE_SYSTEM_NAME iOS CACHE INTERNAL "")
set (UNIX TRUE CACHE INTERNAL "")
set (APPLE TRUE CACHE INTERNAL "")
set (IOS TRUE CACHE INTERNAL "")

set (CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO CACHE INTERNAL "")

# All iOS/Darwin specific settings - some may be redundant
set (CMAKE_SHARED_LIBRARY_PREFIX "lib" CACHE INTERNAL "")
set (CMAKE_SHARED_LIBRARY_SUFFIX ".dylib" CACHE INTERNAL "")
set (CMAKE_SHARED_MODULE_PREFIX "lib" CACHE INTERNAL "")
set (CMAKE_SHARED_MODULE_SUFFIX ".so" CACHE INTERNAL "")
set (CMAKE_MODULE_EXISTS 1 CACHE INTERNAL "")
set (CMAKE_DL_LIBS "" CACHE INTERNAL "")

set (CMAKE_C_OSX_COMPATIBILITY_VERSION_FLAG "-compatibility_version " CACHE INTERNAL "")
set (CMAKE_C_OSX_CURRENT_VERSION_FLAG "-current_version " CACHE INTERNAL "")
set (CMAKE_CXX_OSX_COMPATIBILITY_VERSION_FLAG "${CMAKE_C_OSX_COMPATIBILITY_VERSION_FLAG}" CACHE INTERNAL "")
set (CMAKE_CXX_OSX_CURRENT_VERSION_FLAG "${CMAKE_C_OSX_CURRENT_VERSION_FLAG}" CACHE INTERNAL "")

# Hidden visibilty is required for cxx on iOS 
set (CMAKE_C_FLAGS_INIT "-fembed-bitcode")
set (CMAKE_CXX_FLAGS_INIT "-fvisibility=hidden -fvisibility-inlines-hidden -fembed-bitcode" CACHE INTERNAL "")

set (CMAKE_C_LINK_FLAGS "-Wl,-search_paths_first ${CMAKE_C_LINK_FLAGS}" CACHE INTERNAL "")
set (CMAKE_CXX_LINK_FLAGS "-Wl,-search_paths_first ${CMAKE_CXX_LINK_FLAGS}" CACHE INTERNAL "")

set (CMAKE_PLATFORM_HAS_INSTALLNAME 1 CACHE INTERNAL "")
set (CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-dynamiclib -headerpad_max_install_names" CACHE INTERNAL "")
set (CMAKE_SHARED_MODULE_CREATE_C_FLAGS "-bundle -headerpad_max_install_names" CACHE INTERNAL "")
set (CMAKE_SHARED_MODULE_LOADER_C_FLAG "-Wl,-bundle_loader," CACHE INTERNAL "")
set (CMAKE_SHARED_MODULE_LOADER_CXX_FLAG "-Wl,-bundle_loader," CACHE INTERNAL "")
set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".so" ".a" CACHE INTERNAL "")


# Setup iOS developer location unless specified manually with CMAKE_IOS_DEVELOPER_ROOT
# Note Xcode 4.3 changed the installation location, choose the most recent one available
exec_program(/usr/bin/xcode-select ARGS -print-path OUTPUT_VARIABLE CMAKE_XCODE_DEVELOPER_DIR)
set (XCODE_POST_43_ROOT "${CMAKE_XCODE_DEVELOPER_DIR}/Platforms/${IOS_PLATFORM_LOCATION}/Developer")
set (XCODE_PRE_43_ROOT "/Developer/Platforms/${IOS_PLATFORM_LOCATION}/Developer")
if (NOT DEFINED CMAKE_IOS_DEVELOPER_ROOT)
	if (EXISTS ${XCODE_POST_43_ROOT})
		set (CMAKE_IOS_DEVELOPER_ROOT ${XCODE_POST_43_ROOT})
	elseif(EXISTS ${XCODE_PRE_43_ROOT})
		set (CMAKE_IOS_DEVELOPER_ROOT ${XCODE_PRE_43_ROOT})
	endif (EXISTS ${XCODE_POST_43_ROOT})
endif ()
set (CMAKE_IOS_DEVELOPER_ROOT ${CMAKE_IOS_DEVELOPER_ROOT} CACHE PATH "Location of iOS Platform")


# Find and use the most recent iOS sdk unless specified manually with CMAKE_IOS_SDK_ROOT
if (NOT DEFINED CMAKE_IOS_SDK_ROOT)
	file (GLOB _CMAKE_IOS_SDKS "${CMAKE_IOS_DEVELOPER_ROOT}/SDKs/*")
	if (_CMAKE_IOS_SDKS) 
		list (SORT _CMAKE_IOS_SDKS)
		list (REVERSE _CMAKE_IOS_SDKS)
		list (GET _CMAKE_IOS_SDKS 0 CMAKE_IOS_SDK_ROOT)
	else()
		#message (FATAL_ERROR "No iOS SDK's found in default search path ${CMAKE_IOS_DEVELOPER_ROOT}. Manually set CMAKE_IOS_SDK_ROOT or install the iOS SDK.")
	endif()
	message (STATUS "Toolchain using default iOS SDK: ${CMAKE_IOS_SDK_ROOT}")
endif ()
set (CMAKE_IOS_SDK_ROOT ${CMAKE_IOS_SDK_ROOT} CACHE PATH "Location of the selected iOS SDK")


# Set the sysroot default to the most recent SDK
set (CMAKE_OSX_SYSROOT ${CMAKE_IOS_SDK_ROOT} CACHE PATH "Sysroot used for iOS support")

# Set the find root to the iOS developer roots and to user defined paths
set (CMAKE_FIND_ROOT_PATH ${CMAKE_IOS_DEVELOPER_ROOT} ${CMAKE_IOS_SDK_ROOT} ${CMAKE_PREFIX_PATH} CACHE STRING  "iOS find search path root")

# default to searching for frameworks first
set (CMAKE_FIND_FRAMEWORK FIRST)

# set up the default search directories for frameworks
set (CMAKE_SYSTEM_FRAMEWORK_PATH
	${CMAKE_IOS_SDK_ROOT}/System/Library/Frameworks
	${CMAKE_IOS_SDK_ROOT}/System/Library/PrivateFrameworks
	${CMAKE_IOS_SDK_ROOT}/Developer/Library/Frameworks
)

# only search the iOS sdks, not the remainder of the host filesystem
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


if (LEMONS_IOS_SIMULATOR)
	# Options for building for a simulator

	set (CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "\"iPhone Developer\"" CACHE INTERNAL "")

	set (IOS_PLATFORM_LOCATION "iPhoneSimulator.platform" CACHE INTERNAL "")
	set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphonesimulator" CACHE INTERNAL "")

	set (CMAKE_OSX_ARCHITECTURES "i386;x86_64" CACHE INTERNAL "")

else()
	# Options for building for a real device

	set (CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "<10 character ID>" CACHE INTERNAL "")

	set (IOS_PLATFORM_LOCATION "iPhoneOS.platform" CACHE INTERNAL "")
	set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos" CACHE INTERNAL "")

	set (CMAKE_OSX_ARCHITECTURES "armv7;armv7s;arm64;i386;x86_64" CACHE INTERNAL "")
endif()
