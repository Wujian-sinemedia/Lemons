find_path (FFTW_INCLUDES fftw3.h)

find_library (FFTW_LIBRARIES NAMES fftw3)

find_package_handle_standard_args (FFTW DEFAULT_MSG FFTW_LIBRARIES FFTW_INCLUDES)

if (NOT FFTW_FOUND)
	return()
endif()

# JUCE_DSP_USE_SHARED_FFTW JUCE_DSP_USE_STATIC_FFTW