add_subdirectory (JUCE)

include (ableton-link/AbletonLinkConfig.cmake)

#

if (NOT DEFINED BV_USE_MTS_ESP)
    set (BV_USE_MTS_ESP FALSE)
endif()

if (${BV_USE_MTS_ESP})

add_library (MTS-ESP INTERFACE)

target_sources (MTS-ESP INTERFACE 
	"${bv_sharedcode_dir}/third_party/MTS-ESP/Client/libMTSClient.h"
	"${bv_sharedcode_dir}/third_party/MTS-ESP/Client/libMTSClient.cpp")

target_include_directories (MTS-ESP INTERFACE "${bv_sharedcode_dir}/third_party/MTS-ESP/Client" "MTS-ESP")

target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

endif()

#

if (NOT DEFINED BV_IGNORE_MIPP)
    set (BV_IGNORE_MIPP FALSE)
endif()

if (NOT ${BV_IGNORE_MIPP})

add_library (MIPP INTERFACE)

target_sources (MIPP INTERFACE 
	"${bv_sharedcode_dir}/third_party/MIPP/src/mipp.h")

target_include_directories (MIPP INTERFACE "${bv_sharedcode_dir}/third_party/MIPP/src" "MIPP")

target_compile_definitions (MIPP INTERFACE MIPP_ENABLE_BACKTRACE BV_USE_MIPP=1)
    
endif()
