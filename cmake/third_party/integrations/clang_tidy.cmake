find_program (CLANGTIDY clang-tidy)

if (NOT CLANGTIDY)
    return()
endif()

set (CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})