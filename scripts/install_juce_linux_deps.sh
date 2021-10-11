#!/usr/bin/env bash

# Installs all JUCE Linux dependencies, as described here: https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

sudo apt update

sudo apt install -y --no-install-recommends \
    libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev  \
    libfreetype6-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev \
    lv2-dev  # Also install LV2 development headers on Linux

exit 0
