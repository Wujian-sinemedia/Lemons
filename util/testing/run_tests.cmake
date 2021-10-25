set (CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../..")
set (CTEST_BINARY_DIRECTORY "${CTEST_SOURCE_DIRECTORY}/Builds")

set (CTEST_START_WITH_EMPTY_BINARY_DIRECTORY TRUE)

ctest_empty_binary_directory (${CTEST_BINARY_DIRECTORY})

find_program (CTEST_GIT_COMMAND NAMES git)

set (GITInitSubmodules true)

set (CTEST_UPDATE_COMMAND "${CTEST_GIT_COMMAND}")

set (CTEST_CONFIGURE_COMMAND "${CMAKE_COMMAND} --preset config_tests")

set (DefaultCTestConfigurationType Debug)

ctest_update()
ctest_configure()
ctest_build()
ctest_test()