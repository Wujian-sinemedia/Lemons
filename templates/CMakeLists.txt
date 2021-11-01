add_subdirectory (DefaultJuceApp)
add_subdirectory (DefaultJucePlugin)


add_custom_target (Templates COMMENT "Building templates...")

add_dependencies (Templates NewApp NewPlugin)
