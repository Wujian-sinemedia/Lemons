option (IPP_STATIC "" FALSE)
option (IPP_MULTI_THREADED "" FALSE)

find_path (IPP_INCLUDE_DIR ipp.h PATHS /opt/intel/oneapi/ipp/latest/include)

# so we can restore these later, after changing them...
set (_IPP_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})

if (WIN32)
    set (CMAKE_FIND_LIBRARY_SUFFIXES .lib)
else()
    if (IPP_STATIC)
        set (CMAKE_FIND_LIBRARY_SUFFIXES .a)
    else()
        set (CMAKE_FIND_LIBRARY_SUFFIXES .so)
    endif()
endif()

if (IPP_STATIC)
    if (IPP_MULTI_THREADED)
        set (IPP_LIBNAME_SUFFIX _t)
    else()
        set (IPP_LIBNAME_SUFFIX _l)
    endif()
else()
    set (IPP_LIBNAME_SUFFIX "")
endif()

macro (find_ipp_library IPP_COMPONENT)
  string (TOLOWER ${IPP_COMPONENT} IPP_COMPONENT_LOWER)

  find_library (IPP_LIB_${IPP_COMPONENT} ipp${IPP_COMPONENT_LOWER}${IPP_LIBNAME_SUFFIX}
               PATHS ${IPP_ROOT}/lib/ia32/)
endmacro()

# Core
find_ipp_library (CORE)

# Audio Coding
find_ipp_library (AC)

# Generated Functions
find_ipp_library (GEN)

# Small Matrix Operations
find_ipp_library (M)

# Signal Processing
find_ipp_library (S)

# Vector Math
find_ipp_library (VM)

set (IPP_LIBRARY
    ${IPP_LIB_CORE}
    ${IPP_LIB_AC}
    ${IPP_LIB_GEN}
    ${IPP_LIB_M}
    ${IPP_LIB_S}
    ${IPP_LIB_VM})

set (CMAKE_FIND_LIBRARY_SUFFIXES ${_IPP_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})

find_package_handle_standard_args (IPP DEFAULT_MSG IPP_INCLUDE_DIR IPP_LIBRARY)

if (NOT IPP_FOUND)
    return()
endif()

add_library (IntelIPP INTERFACE)

target_link_libraries (IntelIPP INTERFACE ${IPP_LIBRARY})
target_include_directories (IntelIPP INTERFACE ${IPP_INCLUDE_DIR})
target_compile_definitions (IntelIPP INTERFACE BV_USE_IPP=1)

message (STATUS " -- IPP found! -- ")
