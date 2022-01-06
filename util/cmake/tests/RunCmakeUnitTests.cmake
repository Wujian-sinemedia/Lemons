set (CPM_Lemons_SOURCE @CPM_Lemons_SOURCE@)

include ("${CPM_Lemons_SOURCE}/AddLemons.cmake")


foreach (filepath @TEST_FILE_PATHS@)

	include (${filepath})
	
endforeach()
