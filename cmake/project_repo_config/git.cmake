if (NOT GIT_FOUND)
    find_package (Git QUIET)
endif()

if (NOT GIT_FOUND)
    message (WARNING "Git not found! Some repository configuration features may not work as expected.")
endif()
