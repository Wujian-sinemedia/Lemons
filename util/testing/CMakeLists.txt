add_subdirectory (UnitTestRunner)

add_test (NAME LemonsTests COMMAND UnitTestRunner WORKING_DIRECTORY "${LEMONS_REPO_ROOT}")
