#!/bin/bash

git pull

git submodule foreach 'git fetch && git checkout origin/HEAD && git submodule update --recursive --merge'

git commit -m "submodule auto-update" -a

git push
