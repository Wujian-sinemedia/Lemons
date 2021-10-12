# Lemons utility scripts

This directory contains several shell scripts to perform basic configuration tasks:

## install_deps.sh

This script installs all required dependencies for a typical Lemons workspace, as well as Ccache.

## install_integrations.sh

This script installs all the auxillery integrations for Lemons -- clang format, etc.

## publish_docs.sh

Intended for use by CI machines, this script builds a new version of the documentation and pushes it to the 'docs' branch.
