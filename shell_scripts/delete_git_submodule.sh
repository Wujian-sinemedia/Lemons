#!/bin/bash

# First, delete the relevant entry from the .gitmodules file, then commit that change.

SUBMODULE_PATH=""

git rm --cached $SUBMODULE_PATH

rm -rf .git/modules/$SUBMODULE_PATH

git commit -m "Removed submodule" -a

git push
