# Keeps all project repos' clang format files in sync with the one in this repo
bv_copy_file (${BV_SHAREDCODE_DIR}/_clang-format ${BV_PROJECT_SOURCE_DIR}/_clang-format)

# Configures a git hook to automatically run clang-format over all changed files before each commit
include (git_hooks/GitHooks.cmake)