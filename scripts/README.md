# Lemons utility scripts

This directory contains several shell scripts to perform basic configuration tasks:

## install_deps.sh

This script installs all required dependencies for a typical Lemons workspace, as well as Ccache. 
If you're on Linux, this script will also run 'install_juce_linux_deps.sh'.

## install_juce_linux_deps.sh

This script installs all of JUCE's dependencies for Ubuntu, and the LV2 development headers.

## publish_docs.sh

Intended for use by CI machines, this script builds a new version of the documentation and pushes it to the 'docs' branch.
