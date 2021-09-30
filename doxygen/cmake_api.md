# Lemons CMake API

---

## Functions

### lemons_configure_juce_plugin & lemons_configure_juce_app
Performs a default configuration of a juce app or plugin. These two functions have the same arguments except where differences are noted:

Toggles:
- BROWSER : when mentioned in the call, enables the juce web browser functionality for this target
- PLUGIN_HOST : when mentioned in the call, enables juce audio plugin hosting capabilities for this target
- CAMERA : when mentioned in the call, enables juce support for using the camera with this target

Arguments:
- TARGET : the name of the app or plugin target (usually the name of your product)
- ASSET_FOLDER (optional): the name of the assets folder to use for generating the resources target. The folder can be relative to your project's root; for example, if your folder structure is "YourProject/assets", you can pass `lemons_configure_juce_app (TARGET YourProject ASSET_FOLDER assets)`. This internally calls lemons_add_resources_folder. 
- AAX_PAGETABLE_FILE (optional) : the path to the AAX pagetable file for this plugin. **(Plugins only!)**

Example usage:
```
juce_add_plugin (MyPlugin ...)

lemons_configure_juce_plugin (TARGET MyPlugin BROWSER ASSET_FOLDER assets)
```

### lemons_add_juce_modules
Searches for nested subdirectories in the indicated parent directory, and calls juce_add_module on each of them. 
This is particularly useful if you organize your source tree with all the juce modules in a particular directory.

Takes a single unnamed argument, which is the absolute path to the parent directory to search.

Example usage:
```
# Assuming the folder structure is:
# <repo_root>
#     source
#         modules 
#             my_module_1
#             my_module_2 ...

# You can simply do this from your project's root dir:
lemons_add_juce_modules (${CMAKE_CURRENT_LIST_DIR}/source/modules)
```

### lemons_add_resources_folder
Configures a binary resources target for your project. Internally uses the juce CMake API to create the binaries target.

Arguments:
- TARGET : the name of the product target to link the generated resources target to. The resources target will be named "$TARGET-Assets". If you're making a JUCE plugin, this should be your plugin's shared code target (usually named the same as your product)
- FOLDER : the absolute path of the assets folder to use to populate the resources target. This folder can have any nested directory structure -- glob-resurs'ing is used to generate the list of files.

This is called for you by lemons_configure_juce_plugin/lemons_configure_juce_app if you call those functions with the ASSET_FOLDER argument.

Example usage:
```
lemons_add_resources_folder (TARGET MyProject FOLDER ${CMAKE_CURRENT_LIST_DIR}/assets)
```

### lemons_subdir_list
Returns a list of subdirectories found in the specified parent directory.

Arguments:
- DIR : the absolute path of the parent directory to search
- RESULT : the name of the variable to which the output list will be written in the calling scope

Example usage:
```
lemons_subdir_list (RESULT subdirs DIR /my/directory)

foreach (folder ${subdirs})
     # do something with "folder"...
endforeach()
```
