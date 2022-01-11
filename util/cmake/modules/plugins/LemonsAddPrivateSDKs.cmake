include_guard (GLOBAL)

include (LemonsGetCPM)

if (CPM_PrivateSDKs_SOURCE)
        file (REAL_PATH ${CPM_PrivateSDKs_SOURCE} CPM_PrivateSDKs_SOURCE EXPAND_TILDE)
elseif (DEFINED ENV{LEMONS_PRIVATE_SDKS})
        file (REAL_PATH $ENV{LEMONS_PRIVATE_SDKS} CPM_PrivateSDKs_SOURCE EXPAND_TILDE)
endif()

CPMAddPackage (
        NAME PrivateSDKs
        GITHUB_REPOSITORY benthevining/PrivateSDKs
        GIT_TAG origin/main)

mark_as_advanced (FORCE LEMONS_AAX_SDK_PATH LEMONS_VST2_SDK_PATH)
