#!/usr/bin/env python

import os
import shutil
import re

###############################################################################

# Copies the readme from the cmake directory into the working tree for doxygen

def copy_cmake_readme (script_dir, dest_dir):

    filename = "cmake_api.md"

    orig_file_path = os.path.join (script_dir, filename)

    if not os.path.exists (orig_file_path):
        raise Exception("CMake API readme not found!")
        return

    dest_file_path = os.path.join (dest_dir, filename)

    shutil.copy2 (orig_file_path, dest_file_path)


# Copies the relevant sections from the top-level readme into the main_page.md file

def copy_from_main_readme (source_dir, script_dir, dest_dir):

    filename = "main_page.md"

    orig_file_path = os.path.join (script_dir, filename)

    if not os.path.exists (orig_file_path):
        raise Exception("Main Lemons readme not found!")
        return

    dest_file_path = os.path.join (dest_dir, filename)

    shutil.copy2 (orig_file_path, dest_file_path)

    with open (os.path.join (source_dir, "README.md"), "r") as f:
        orig_readme_text = f.read()

    last_bit_of_intro = "whatever floats your git boat."

    relevant_text = orig_readme_text.split (last_bit_of_intro, 1)[1]

    if not relevant_text:
        raise Exception("No relevant text found in main Lemons readme")
        return

    with open (dest_file_path, "a") as f:
        f.write (relevant_text)


###############################################################################

# Copies the logo from the assets dir into the build tree

def copy_logo (script_dir, dest_dir):

    filename = "logo.png"

    util_dir = os.path.abspath (os.path.dirname (script_dir))
    assets_dir = os.path.join (util_dir, "assets")

    orig_logo_file = os.path.join (assets_dir, filename)

    if not os.path.exists (orig_logo_file):
        raise Exception("Logo not found")
        return

    shutil.copy2 (orig_logo_file, 
                  os.path.join (dest_dir, filename))

###############################################################################

# Copies all needed files to the build tree

def copy_all_needed_files (lemons_root, doxygen_dir, dest_dir):

    copy_cmake_readme (doxygen_dir, dest_dir)

    copy_logo (doxygen_dir, dest_dir)

    copy_from_main_readme (lemons_root, doxygen_dir, dest_dir)

