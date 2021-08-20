# Shared-code
A library of utilities and building blocks for JUCE-based apps and plugins

This library is what I use to build my own projects, so hopefully you find it useful as well.

It's recommended to build with CMake. You could manually add the desired modules to a Projucer project, but CMake is a better build system anyway. 
What I usually do in project repos is make my [UsefulScripts](https://github.com/BenViningMusicSoftware/UsefulScripts) repo a git submodule, then from the project's main CMakeLists.txt, simply `addSubdirectory(UsefulScripts)`.
For an example of a project repo that uses this configuration, see [Imogen's CMakeLists.txt](https://github.com/benthevining/imogen/blob/main/CMakeLists.txt).
