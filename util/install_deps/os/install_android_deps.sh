#!/usr/bin/env bash

sudo apt-get install openjdk-8-jre

# sudo update-alternatives --config java

sudo apt-get install openjdk-8-jdk

# sudo update-alternatives --config javac

sudo apt-get install android-sdk sdkmanager

yes | sudo sdkmanager --licenses

sdkmanager "platforms;android-27"

exit 0
