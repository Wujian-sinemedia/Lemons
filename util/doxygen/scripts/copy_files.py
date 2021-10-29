#!/usr/bin/env python

from os import path
import shutil

###############################################################################

# Copies the relevant sections from the top-level readme into the main_page.md file

def copy_from_main_readme (source_dir, script_dir, dest_dir):

    filename = "main_page.md"

    orig_file_path = path.join (script_dir, filename)

    if not path.exists (orig_file_path):
        raise Exception("Main Lemons readme not found!")
        return

    dest_file_path = path.join (dest_dir, filename)

    shutil.copy2 (orig_file_path, dest_file_path)

    with open (path.join (source_dir, "README.md"), "r") as f:
        orig_readme_text = f.read()

    last_bit_of_intro = "whatever floats your git boat."

    relevant_text = orig_readme_text.split (last_bit_of_intro, 1)[1]

    if not relevant_text:
        raise Exception("No relevant text found in main Lemons readme")
        return

    with open (dest_file_path, "a") as f:
        f.write (relevant_text)

###############################################################################

# Copies all needed files to the build tree

def copy_all_needed_files (lemons_root, doxygen_dir, dest_dir):

    copy_from_main_readme (lemons_root, doxygen_dir, dest_dir)

