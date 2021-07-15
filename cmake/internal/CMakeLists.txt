include (configure_helpers.cmake)
include (plugin_format_helpers.cmake)

if (APPLE)
	include (native/Apple.cmake)
elseif (WIN32)
	include (native/Windows.cmake)
endif()

set (BV_BRAND_FILE "${CMAKE_CURRENT_LIST_DIR}/BrandSpecific.cmake" CACHE INTERNAL "Path to the BenViningMusicSoftware brand-specific config file.")

#

function (_bv_add_resources_folder target folder)
    set (resourcesTarget ${target}-Assets)

    if (NOT TARGET ${resourcesTarget})
        file (GLOB_RECURSE files "${folder}/*.*")
        juce_add_binary_data (${resourcesTarget} SOURCES ${files})
        set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
        target_compile_definitions (${resourcesTarget} INTERFACE BV_HAS_BINARY_DATA=1)
    endif()

    target_link_libraries (${target} PRIVATE ${resourcesTarget})
endfunction()

#

function (_bv_subdir_list result curdir)
  	file (GLOB children RELATIVE ${curdir} ${curdir}/*)

  	set (dirlist "")

  	foreach (child ${children})
    	if (IS_DIRECTORY ${curdir}/${child})
      		list (APPEND dirlist ${child})
    	endif()
  	endforeach()

  	set (${result} ${dirlist} PARENT_SCOPE)
endfunction()

#

function (_bv_add_juce_modules_internal dir list)
	_bv_subdir_list (moduleFolders ${dir})

    foreach (folder ${moduleFolders})
        juce_add_module (${dir}/${folder})
    endforeach()

    list (APPEND ${list} ${moduleFolders} PARENT_SCOPE)
endfunction()