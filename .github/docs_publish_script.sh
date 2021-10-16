#!/bin/sh


readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"


# install deps
source "$script_dir/../scripts/install/install_list.sh"
install_deps_list "doxygen_deps.txt"


# Run python script
python "$script_dir/doxygen/publish_docs.py"


exit 0
