#!/bin/sh


readonly repo_url="github.com/benthevining/Lemons.git"

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

readonly working_dir="$script_dir/Lemons"


# install deps
source "$script_dir/../scripts/install/install_list.sh"
install_deps_list "doxygen_deps.txt"


# Run python script

python "$script_dir/../doxygen/publish_docs.py"

exit 0
