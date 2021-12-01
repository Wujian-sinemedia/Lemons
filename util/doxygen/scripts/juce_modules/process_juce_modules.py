#!/usr/bin/env python

import os
import shutil

from process_juce_module import process_juce_module

from argparse import ArgumentParser

#

def get_category_description_from_cmake_module (dir_path):
    for dirpath, dirnames, filenames in os.walk (dir_path):
        for file in filenames:
            root, ext = os.path.splitext (file)
            if ext == ".cmake":
                return root

    return ""

#

def process_category_description (dir_path):

    category_cmake_module = get_category_description_from_cmake_module (dir_path)

    if not category_cmake_module:
        raise Exception("Can't find CMake module in this JUCE module category directory!")

    description = ""

    with open (os.path.join (dir_path, category_cmake_module + ".cmake"), "r") as f:
        for line in f:
            if line.startswith ("#[["): continue
            description = line.strip()
            break

    if not description:
        raise Exception("Description could not be parsed!")

    cmakeInfo = "To use these JUCE modules, include the [{n}](@ref {n}) CMake module, and link against the `{n}` target.".format(n=category_cmake_module)

    return description, cmakeInfo

#

def process_module_category (category_name, orig_cat_dir, dest_cat_dir):

    if not os.path.isdir (orig_cat_dir):
        raise Exception("Module category dir does not exist!")
        return

    if not category_name:
        raise Exception("Module category name cannot be empty!")
        return

    if os.path.isdir (dest_cat_dir):
        shutil.rmtree (dest_cat_dir)

    shutil.copytree (orig_cat_dir, dest_cat_dir)

    category_info = process_category_description (dest_cat_dir)

    category_definiton = []
    category_definiton.append ("/** @defgroup {n} {n}".format (n=category_name))

    category_definiton.append ("    {d}".format (d=category_info[0]))
    category_definiton.append ("")
    category_definiton.append ("    {l}".format (l=category_info[1]))

    del category_info

    category_definiton.append ("")
    category_definiton.append ("    @{")
    category_definiton.append ("*/")

    for subdir in os.listdir  (dest_cat_dir):
        module_path = os.path.join (dest_cat_dir, subdir)
        if os.path.isdir (module_path):
            category_definiton.append ("")
            category_definiton.append (process_juce_module (category_name, subdir, module_path))

    category_definiton.append ("")
    category_definiton.append ("/** @} */")

    return "\r\n".join (category_definiton)


#

def create_module_heirarchy (source_dir, dest_dir):

    if not os.path.isdir (source_dir):
        raise Exception("Juce modules source directory does not exist!")
        return

    orig_module_dir = os.path.join (source_dir, "modules")

    if not os.path.isdir (orig_module_dir):
        raise Exception("Juce modules source directory does not exist!")
        return

    category_definitions = []

    for category in os.listdir (orig_module_dir):
        category_path = os.path.join (orig_module_dir, category)
        if os.path.isdir (category_path):
            category_definitions.append (process_module_category (category, category_path,
                                                                  os.path.join (dest_dir, category)))

    # Create a .dox file containing the entire module hierarchy
    with open (os.path.join (dest_dir, "lemons_modules.dox"), "w") as f:
        f.write ("\r\n\r\n".join (category_definitions))

#

if __name__ == "__main__":

    parser = ArgumentParser()
    parser.add_argument ("lemons_root", help="the absolute path to the Lemons repo root")
    parser.add_argument ("dest_dir", help="the absolute path to the output")
    
    args = parser.parse_args()

    create_module_heirarchy (args.lemons_root, args.dest_dir)
