find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

add_custom_target (ClangFormat
                   COMMAND ${CLANG_FORMAT}
                   COMMENT "running clang format")

message (STATUS " -- using clang-format! -- ")