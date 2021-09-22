### clang-tidy ###

find_program (CLANGTIDY clang-tidy)

if (CLANGTIDY)
    message (STATUS " --- using clang-tidy --- ")

    set (CMAKE_CXX_CLANG_TIDY "${CLANGTIDY};-checks=cppcoreguidelines-*")

    if (WIN32) # for MSVC
        set (CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};--extra-arg-before=-fms-compatibility-version=19.10")
    endif()
endif()


### cppcheck ###

find_program (CPPCHECK_EXE cppcheck)

if (CPPCHECK_EXE)
    message (STATUS " --- using cppcheck --- ")

    set (CMAKE_CXX_CPPCHECK
        ${CPPCHECK_EXE}
        #--suppressions-list=${CMAKE_SOURCE_DIR}/misc/cppcheck-suppressions.txt
        --inline-suppr
        -q
        --enable=all
        --force
        --std=c++20
        -I ${CMAKE_SOURCE_DIR}
        --template="cppcheck: warning: {id}:{file}:{line}: {message}"
        -i src/third_party)
endif()
