find_program (CPPCHECK_EXE cppcheck)

if (NOT CPPCHECK_EXE)
    return()
endif()

set (CMAKE_CXX_CPPCHECK
    ${CPPCHECK_EXE}
    #--suppressions-list=${CMAKE_SOURCE_DIR}/misc/cppcheck-suppressions.txt
    --inline-suppr
    -q
    --enable=all
    --force
    --std=c++20
    -I ${CMAKE_SOURCE_DIR}
    #--template="cppcheck: warning: {id}:{file}:{line}: {message}"
    -i src/third_party)