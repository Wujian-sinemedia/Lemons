find_program (CLANGTIDY clang-tidy)

if (CLANGTIDY)
    set (CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
endif()
