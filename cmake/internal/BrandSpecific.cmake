include_guard (GLOBAL)

#[[
	This file sets some flags that are specific to my brand's plugins and apps.
	Author: Ben Vining
]]


set (BV_DEFAULT_BRAND_FLAGS
	BUNDLE_ID                   com.BenViningMusicSoftware.${CMAKE_PROJECT_NAME}
	COMPANY_NAME                BenViningMusicSoftware
    COMPANY_WEBSITE             www.benvining.com
    COMPANY_EMAIL               ben.the.vining@gmail.com
    COMPANY_COPYRIGHT           "Copyright 2021 Ben Vining"
    PLUGIN_MANUFACTURER_CODE	Benv
	CACHE INTERNAL "BenViningMusicSoftware brand flags")