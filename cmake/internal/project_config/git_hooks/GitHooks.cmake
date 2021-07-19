if (NOT GIT_FOUND)
  find_package (Git QUIET)
endif()

if (NOT CLANG_FORMAT)
  find_program (CLANG_FORMAT clang-format)
endif()

if (NOT GIT_FOUND OR NOT CLANG_FORMAT)
    return()
endif()

if (EXISTS ${PROJECT_SOURCE_DIR}/.git)
    message (STATUS "Configuring clang-format git hook...")

   configure_file (${CMAKE_CURRENT_LIST_DIR}/pre_commit.in
                   ${PROJECT_SOURCE_DIR}/.git/hooks/pre-commit)
endif()
