#!/bin/sh

# This script is based on: https://gist.github.com/vidavidorra/548ffbcdae99d752da02
# This is intended for CI machines automatically building & pushing new versions of the docs onto the 'docs' branch, for publishing on the GitHub Pages site.

# input:
# - GH_REPO_TOKEN	: Secure token to the github repository.

set -euo pipefail

#

readonly repo_url="github.com/benthevining/Lemons.git"

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

readonly doxygen_dir="$script_dir/../doxygen"

readonly working_dir="$doxygen_dir/Lemons"


# install deps
source "$script_dir/install/install_list.sh"
install_deps_list "doxygen_deps.txt"


# Clone the docs branch

cd "$doxygen_dir"

make clean

echo "Cloning docs branch..."
git clone -b docs "https://$repo_url"

cd "$working_dir"

# remove everything currently in the docs branch
rm -rf *

# Build new copy of docs
cd "$doxygen_dir"
echo "Building documentation..."
make

# Copy new docs into 'docs' branch
mv $doxygen_dir/doc/* "$working_dir"

cd "$working_dir"

# Need to create a .nojekyll file to allow filenames starting with an underscore
# to be seen on the gh-pages site. Therefore creating an empty .nojekyll file.
echo "" > .nojekyll

echo "Publishing documentation..."

git config --global push.default simple
git config user.name "Github Actions"
git config user.email "actions@github.com"

git commit -a -m "Updating documentation"

git push --force --allow-empty "https://$GH_REPO_TOKEN@$repo_url"

exit 0
