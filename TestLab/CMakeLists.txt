project (TestLab VERSION 0.0.1 LANGUAGES CXX)

juce_add_gui_app (${PROJECT_NAME}
    ${BV_DEFAULT_BRAND_FLAGS}
    PRODUCT_NAME					${PROJECT_NAME}
    VERSION                     	${PROJECT_VERSION}
    DESCRIPTION                     ""
    NEEDS_MIDI_INPUT            	TRUE
    NEEDS_MIDI_OUTPUT           	TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS 	FALSE
    MICROPHONE_PERMISSION_ENABLED   TRUE
    MICROPHONE_PERMISSION_TEXT      ""
    DOCUMENT_BROWSER_ENABLED    	TRUE
    BACKGROUND_AUDIO_ENABLED		FALSE # for iOS
    NEEDS_CURL                  	TRUE
    NEEDS_WEB_BROWSER           	TRUE
    ICON_BIG                    	""  # path to icon file
    STATUS_BAR_HIDDEN           	TRUE  # for iOS
    REQUIRES_FULL_SCREEN        	TRUE  # for iOS
    IPAD_SCREEN_ORIENTATIONS    	UIInterfaceOrientationUnknown, UIInterfaceOrientationLandscapeLeft, UIInterfaceOrientationLandscapeRight
    TARGETED_DEVICE_FAMILY      	2     # target iPad only
    DOCUMENT_EXTENSIONS         	xml
    SEND_APPLE_EVENTS_PERMISSION_ENABLED FALSE
    )

lemons_add_juce_modules ("${CMAKE_CURRENT_LIST_DIR}/modules")

lemons_configure_juce_app (TARGET ${PROJECT_NAME} ASSET_FOLDER assets PLUGIN_MODULES PLUGIN_HOST)

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries (${PROJECT_NAME} PRIVATE new_app_gui)
