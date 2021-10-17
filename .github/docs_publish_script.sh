#!/bin/sh

readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# install deps
bash "$script_dir/../install_deps/install_deps.sh"

# Run python script
python "$script_dir/publish_docs.py"

exit 0
