include_guard (GLOBAL)

include (LemonsGetCPM)

if (CPM_PrivateSDKs_SOURCE)
        file (REAL_PATH ${CPM_PrivateSDKs_SOURCE} CPM_PrivateSDKs_SOURCE EXPAND_TILDE)
endif()

CPMAddPackage (
        NAME PrivateSDKs
        GITHUB_REPOSITORY benthevining/PrivateSDKs
        GIT_TAG origin/main)
