#!/bin/sh

# This script is based on: https://gist.github.com/vidavidorra/548ffbcdae99d752da02
# This is intended for CI machines automatically building & pushing new versions of the docs onto the 'docs' branch, for publishing on the GitHub Pages site.

# input:
# - GH_REPO_TOKEN	: Secure token to the github repository.

set -euo pipefail

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# install deps
source "$script_dir/install/install_list.sh"
install_deps_list "doxygen_deps.txt"

readonly doxygen_dir="$script_dir/../doxygen"

# Clone the docs branch
cd "$doxygen_dir"
echo "Cloning docs branch..."
git clone -b docs https://github.com/benthevining/Lemons.git

readonly working_dir="$doxygen_dir/Lemons"

cd "$working_dir"

# remove everything currently in the docs branch
rm -rf *

git config --global push.default simple
git config user.name "Github Actions"
git config user.email "actions@github.com"

# Build new copy of docs
cd "$doxygen_dir"
echo "Building documentation..."
make 2>&1 | tee "$working_dir/doxygen_output.log"

# Copy new docs into 'docs' branch
mv $doxygen_dir/doc/* "$working_dir"

cd "$working_dir"

# Need to create a .nojekyll file to allow filenames starting with an underscore
# to be seen on the gh-pages site. Therefore creating an empty .nojekyll file.
echo "" > .nojekyll

echo "Publishing documentation..."

git commit -a -m "Updating documentation"
git push --force "https://${GH_REPO_TOKEN}@github.com/benthevining/Lemons.git" > /dev/null 2>&1

exit 0
