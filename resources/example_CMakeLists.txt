#[[
	An example CMakeLists.txt file for a "workspace" set up on a development machine. 
	The Shared-code repo is designed with the use case of development of multiple products which all use Shared-code, while also editing Shared-code itself, in mind.
	The suggested setup for said use case is to create a directory to house the repos you will be working on ("Workspace"), and add a CMakeLists.txt in this top-level superdirectory:

	Workspace
		- CMakeLists.txt
		- Shared-code (your local copy of the Shared-code repo)
		- Project1 (this is a git repo!)
		- Project2 (this is a git repo!)
		- Project3 (this is a git repo!)

	Below is the suggested setup for the CMakeLists.txt file in the top-level "Workspace" directory.
	This file doesn't need to be committed to any repository, it is simply a configuration of your local build environment.
]]

cmake_minimum_required (VERSION 3.17)

project (Workspace)

# Tells CPM to cache the sources for all dependencies (and the CPM script itself). You should only have to download them once. 
# To update a dependency, simply delete it from the Cache folder and it should redownload the next time you run the CMake configuration.
set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/Cache)

# Tells all the added subprojects to use the version of the Shared-code source from the local copy of the repo.
set (CPM_Shared-code_SOURCE ${CMAKE_CURRENT_LIST_DIR}/Shared-code)

# If you have JUCE installed elsewhere on your computer, include this line. 
# Otherwise, omit this line and CPM will download JUCE into the Cache folder the first time the configuration is run.
set (CPM_JUCE_SOURCE /path/to/juce/on/your/computer)

add_subdirectory (Project1)
add_subdirectory (Project2)
add_subdirectory (Project3)