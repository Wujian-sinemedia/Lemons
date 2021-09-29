#!/bin/sh

# This script is based on: https://gist.github.com/vidavidorra/548ffbcdae99d752da02

# input:
# - GH_REPO_TOKEN	: Secure token to the github repository.

set -euo pipefail

# install deps
apt-get install -y --no-install-recommends doxygen graphviz

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd "$script_dir"
echo "Cloning docs branch..."
git clone -b docs https://github.com/benthevining/Lemons.git

readonly working_dir="$script_dir/Lemons"

cd "$working_dir"

# remove everything currently in the docs branch
rm -rf *

git config --global push.default simple
git config user.name "Github Actions"
git config user.email "actions@github.com"

cd "$script_dir"
echo "Building documentation..."
make 2>&1 | tee "$working_dir/doxygen_output.log"

mv $script_dir/doc/* "$working_dir"

cd "$working_dir"

# Need to create a .nojekyll file to allow filenames starting with an underscore
# to be seen on the gh-pages site. Therefore creating an empty .nojekyll file.
echo "" > .nojekyll

echo "Publishing documentation..."

git add --all

git commit -m "Updating documentation"

git push --force "https://${GH_REPO_TOKEN}@github.com/benthevining/Lemons.git" > /dev/null 2>&1

exit 0
