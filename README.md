# Shared-code
### *A library of utilities and building blocks for JUCE-based apps and plugins*

This library is what I use to build my own projects, so hopefully you find it useful as well.

Contributing is welcome and appreciated! *(Or feel free to just make feature suggestions/requests.)* You may fork this repo, file issues, make pull requests, send me hate mail -- whatever floats your git boat.

It's recommended to build with CMake. You could manually add the desired modules to a Projucer project, but CMake is a better build system anyway. 

When you add the Shared-code subdir in your CMake, this repo's cmake scripts will automatically configure your build to use [ccache](https://ccache.dev/) as the compiler launcher, if the ccache program can be located on your machine. To install it, grab a release from [this repo](https://github.com/cristianadam/ccache).

I also recommend that within your CMake, you add Shared-code by using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake), which is a single-script CMake package manager that allows you lots of flexibility in project configuration, such as specifying that a local directory contains the source for a certain package (ie, so that not every project repository has to contain Shared-code as a submodule. This also means that, for your entire machine, you only need **one** copy of JUCE, no matter how many repositories want to refer to it).

What I usually do in project repos is make my [UsefulScripts](https://github.com/BenViningMusicSoftware/UsefulScripts) repo a git submodule, then from the project's main CMakeLists.txt, simply `addSubdirectory(UsefulScripts)`. What this will do is:
* Either fetch the CPM.cmake script from the github repository and cache it for future use; ***or*** include the previously-cached version if it can be found
* Include the Shared-code repository using CPM.cmake
and a couple other boilerplate things that are useful for my own usage in my projects...

For an example of a project repo that uses this configuration, see [Imogen's CMakeLists.txt](https://github.com/benthevining/imogen/blob/main/CMakeLists.txt).

For an example of how you can integrate several project repos of this nature side-by-side in a "super-repo", check out my [dev_workspace repo](https://github.com/BenViningMusicSoftware/dev_workspace) -- this is the only repo I have to clone onto a dev machine in order to work on all my projects!
