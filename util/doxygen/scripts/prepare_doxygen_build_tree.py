#!/usr/bin/env python

import os
import shutil
import re

from process_modules import create_module_heirarchy
from copy_files import copy_all_needed_files

###############################################################################

# Main script 

if __name__ == "__main__":

    script_dir = os.path.abspath (os.path.dirname (__file__))
    doxygen_dir = os.path.abspath (os.path.dirname (script_dir))

    dest_dir = os.path.join (doxygen_dir, "build")

    if os.path.isdir (dest_dir):
        shutil.rmtree (dest_dir)

    os.mkdir (dest_dir)

    lemons_root = os.path.abspath (os.path.dirname (os.path.dirname (doxygen_dir)))

    copy_all_needed_files (lemons_root, doxygen_dir, dest_dir)

    create_module_heirarchy (lemons_root, dest_dir)
