include_guard (GLOBAL)

# fixes a bug with LTO on Ubuntu with Clang
set (
	CMAKE_AR
	${CMAKE_CXX_COMPILER_AR}
	CACHE PATH "AR" FORCE)
set (
	CMAKE_RANLIB
	${CMAKE_CXX_COMPILER_RANLIB}
	CACHE PATH "RANLIB" FORCE)

mark_as_advanced (CMAKE_AR CMAKE_RANLIB)
