if (DEFINED IgnoreVDSP)
	set (BV_IGNORE_VDSP ${IgnoreVDSP} CACHE INTERNAL "Ignore Apple vDSP for this build?")
else()
	set (BV_IGNORE_VDSP FALSE CACHE INTERNAL "Ignore Apple vDSP for this build?")
endif()


# Formats