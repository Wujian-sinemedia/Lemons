include_guard (GLOBAL)

include (LemonsCcache)
include (LemonsClangFormat)
include (LemonsClangTidy)
include (LemonsCppCheck)
include (LemonsCppLint)
include (LemonsIncludeWhatYouUse)

if (LEMONS_SANITIZERS_TO_ENABLE)
	include (LemonsSanitizers)
endif()
