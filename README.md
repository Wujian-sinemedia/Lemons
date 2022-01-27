<!-- markdownlint-disable MD013 MD033 -->

# Lemons

[![Test and release](https://github.com/benthevining/Lemons/actions/workflows/tests.yml/badge.svg)](https://github.com/benthevining/Lemons/actions/workflows/tests.yml)
[![Templates](https://github.com/benthevining/Lemons/actions/workflows/templates.yml/badge.svg)](https://github.com/benthevining/Lemons/actions/workflows/templates.yml)
[![Utils](https://github.com/benthevining/Lemons/actions/workflows/utils.yml/badge.svg)](https://github.com/benthevining/Lemons/actions/workflows/utils.yml)
[![Docs](https://github.com/benthevining/Lemons/actions/workflows/publish_docs.yml/badge.svg)](https://github.com/benthevining/Lemons/actions/workflows/publish_docs.yml)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/benthevining/Lemons/main.svg)](https://results.pre-commit.ci/latest/github/benthevining/Lemons/main)

<p align="center">
  <img src="https://github.com/benthevining/Lemons/blob/main/util/assets/lemons.jpg" alt="Watercolor painting of some lemons" width="1191" height="585"/>
</p>

I used to work full time in live theatre. Then, COVID happened.
I lost my career overnight, and I wasn't really sure what to do. I drifted for a while -- I made some [angsty experimental electronic music](https://soundcloud.com/benvining/american-dream), and then I made some [calmer experimental electronic music](https://soundcloud.com/benvining/sets/introspectralism) -- and eventually, I started dabbling in creating my own electronic instruments in Max MSP. That eventually led me to the JUCE framework, which was my introduction to C++ more generally.

I quickly learned to love JUCE, but I also quickly learned that there was quite a lot of overhead involved in just getting a simple project up and running *(reliably)*. I realized I needed to abstract that overhead into a single repo. Here it is.

Life handed me a bunch of lemons...

wait, damn it, perhaps I should've gone with *Lemonade*, now that I think about it... But, I've already changed all the variable names, and I don't want to step on Beyonce's toes, so, lemons it is.

Here are my lemons, go make lemonade of your own.

<p align="center">
  <img src="https://github.com/benthevining/Lemons/blob/main/util/assets/deke.png" alt="Deke Shaw, from Agents of Shield, saying, 'it's lemons then'." width="409" height="311"/>
</p>

---

## A library of utilities and building blocks for JUCE-based apps and plugins

This library is what I use to build my own projects, so hopefully you'll find it useful as well.

The online documentation can be found [here](https://benthevining.github.io/Lemons/); you can also build an offline copy by running `make docs` -- they'll be output to `util/doxygen/doc`.

Contributing is welcome and appreciated! *(Or feel free to just make feature suggestions/requests.)* You may fork this repo, file issues, make pull requests, send me hate mail -- whatever floats your git boat.

---

## Building

It's recommended to build with CMake. You could manually add the desired modules to a Projucer project, but CMake is a much better build system. Lemons contains several useful CMake scripts that should make getting things running fairly painless...

The full CMake API reference can be found [here](https://benthevining.github.io/Lemons/cmake_api.html).

### Adding with CPM.cmake

I recommend that within your CMake, you add Lemons by using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake), which is a single-script CMake package manager that allows you lots of flexibility in project configuration. One particularly important feature is the ability to specify that a local directory contains the source code for a certain package -- so that not every project repository on your machine has to contain a copy of Lemons as a submodule. This makes your cmake perfectly modular: simply add the Lemons package with CPM.cmake in each project, and then if that project is cloned and built in isolation, CPM will download the Lemons package at configure time, but you also have the ability to tell CPM to reference a local copy of Lemons on your machine, so you can work on all your projects while only having one copy of Lemons on your machine.

Adding Lemons with CPM.cmake looks like this:

```cmake
include (CPM.cmake)

CPMAddPackage (
        NAME Lemons
        GIT_REPOSITORY https://github.com/benthevining/Lemons.git
        GIT_TAG origin/main)
```

### A note about adding JUCE

Internally, Lemons's cmake scripts [add JUCE like this](https://github.com/benthevining/Lemons/blob/main/util/cmake/modules/general/LemonsJuceUtilities.cmake), using CPM.cmake. JUCE will automatically be fetched for you at configure time -- but, if you'd like to reference another local copy of JUCE, you can set this variable before adding the Lemons package:

```cmake
set (CPM_JUCE_SOURCE /absolute/path/to/your/copy/of/juce)
```

Note that this is optional, and if you omit the above line, adding the Lemons package will automatically also add the latest copy of JUCE for you.

### What I actually do in my projects' CMake

I noticed myself duplicating several lines of CMake code in every project, so I encapsulated the *adding of Lemons* into [its own little repository](https://github.com/benthevining/GetLemons).

What I usually do in my project repos is make my [GetLemons](https://github.com/benthevining/GetLemons) repo a git submodule, then from the project's main CMakeLists.txt, simply

```cmake
addSubdirectory (GetLemons)
```

What this will do is:

* Either fetch the CPM.cmake script from the github repository and cache it for future use; ***or*** include the previously-cached version if it can be found
* Include the Lemons repository using CPM.cmake

You can check out the GetLemons repo's ReadMe for more details.

For an example of a project repo that uses this configuration, see [Imogen's CMakeLists.txt](https://github.com/benthevining/imogen/blob/main/CMakeLists.txt).

---

## Integrations

When you add the Lemons subdirectory in your CMake (or GetLemons), my cmake scripts will automatically configure several integrations, if the respective programs can be found on your computer. If they can't be found, cmake will not install anything at configure time. See the section below on installing dependencies and integrations.

### Ccache

My cmake scripts will automatically configure your build to use [ccache](https://ccache.dev/) as the compiler launcher, if the ccache program can be located on your machine. This will drastically speed up consecutive builds! Ccache works by detecting if the same compilation is being repeated, and if so, reusing the old output. For an example of a GithubActions workflow that utilizes a persistent cache, see [here](https://github.com/benthevining/dev_workspace/blob/main/.github/workflows/Build.yml).

### Clang-format

If clang-format is found at configure time, my cmake scripts will automatically add a target to your project called *ClangFormat*, that will recursively run clang-format on all nested subdirectories within your source tree. For this to work, before adding Lemons in your CMake, set the variable *LEMONS_TOPLEVEL_DIR* to the absolute path of your top-level source directory.

### Clang-tidy

(in progress...)

---

## Example dev environment

For an example of how you can integrate several project repos of this nature side-by-side in a "super-repo", check out my [dev_workspace repo](https://github.com/benthevining/dev_workspace) -- this is the only repo I have to clone onto a dev machine in order to work on all my projects, and I only have to have one copy of Lemons and JUCE on my machine, which every project repo will reference.

---

## Licensing

This repository is licensed with the GNU General Public License. This means that any projects using this repository's code ***must also themselves be open-source and licensed under the GPL.***

For inquiries about exemptions for commercial usage, please email me at ben.the.vining@gmail.com.

---

**Happy lemonade making!**
