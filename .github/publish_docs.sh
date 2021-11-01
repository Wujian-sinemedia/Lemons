#!/usr/bin/env bash

# This script is based on: https://gist.github.com/vidavidorra/548ffbcdae99d752da02

# inputs:
# GH_REPO_TOKEN

set -euo pipefail

readonly GH_REPO_REF="github.com/benthevining/Lemons.git"

readonly temp_dir="docs"

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

readonly lemons_root="$script_dir/.."

# install deps
/usr/bin/env bash "$lemons_root/util/install_deps/install_deps.sh"

cd "$script_dir"

# create a clean working directory for the script
mkdir "$temp_dir"
cd "$temp_dir"

# clone the docs branch
git clone -b docs --recurse-submodules "https://git@$GH_REPO_REF"

readonly docs_git_tree="$script_dir/$temp_dir/Lemons"

cd "$docs_git_tree"

# configure git
git config --global push.default simple
git config user.name "Github Actions"
git config user.email "actions@github.com"

# remove everything currently in the docs branch
rm -rf *

# need to create an empty .nojekyll file
echo "" > .nojekyll

# generate the doxygen documentation
cd "$lemons_root"
make docs

# copy generated docs to cloned copy of docs git tree
mv "$lemons_root"/Builds/docs/util/doxygen/doc/* "$docs_git_tree"

cd "$docs_git_tree"

git add --all

git commit -am "Updating docs" 

git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}"

exit 0
