#!/bin/sh

# This script is based on: https://gist.github.com/vidavidorra/548ffbcdae99d752da02

# inputs:
# GH_REPO_TOKEN

set -euo

readonly GH_REPO_REF="github.com/benthevining/Lemons.git"

script_dir="$(cd "$(dirname "$0")" && pwd)"
readonly script_dir

readonly temp_dir="$script_dir/docs"

if [ -d "$temp_dir" ]; then
    rm -rf "$temp_dir"
fi

mkdir "$temp_dir"
cd "$temp_dir"

# clone the docs branch
git clone -b docs --recurse-submodules "https://git@$GH_REPO_REF"

readonly docs_git_tree="$temp_dir/Lemons"

readonly lemons_root="$script_dir/.."

# generate the doxygen documentation
cd "$lemons_root"
make docs

# need to create an empty .nojekyll file
echo "" > .nojekyll

cd "$docs_git_tree"

# remove everything currently in the docs branch
rm -rf -- *

# copy generated docs to cloned copy of docs git tree
# shellcheck disable=SC2086
mv $lemons_root/util/doxygen/doc/* "$docs_git_tree"

# configure git
git config push.default simple
git config user.name "Github Actions"
git config user.email "actions@github.com"

git add --all

git commit -am "Updating docs"

git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}"

exit 0
