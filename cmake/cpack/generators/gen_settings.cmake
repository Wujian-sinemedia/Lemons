if (NOT CPACK_RESOURCE_FILE_LICENSE)
    #set (CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE.txt)
endif()

if (NOT EXISTS ${CPACK_RESOURCE_FILE_LICENSE})
  #message (AUTHOR_WARNING
  #  "${CPACK_RESOURCE_FILE_LICENSE} file not found, provide one or set "
  #  "CPACK_RESOURCE_FILE_LICENSE to point to an existing one.")
endif()