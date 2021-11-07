#!/usr/bin/env python

import os
import shutil

from argparse import ArgumentParser

from process_juce_module_header import process_module_header

###############################################################################

# Add a Doxygen group to the file at 'path'

def add_doxygen_group (path, group_name):

    if not os.path.exists (path):
        raise Exception("Cannot add Doxygen group to nonexistent filepath!")
        return

    if not group_name:
        raise Exception("Doxygen group name cannot be empty!")
        return

    try:
        with open (path, "r") as f:
            content = f.read()
    except UnicodeDecodeError:
        return

    with open (path, "w") as f:
        f.write ("\r\n/** @ingroup " + group_name + "\r\n *  @{\r\n */\r\n")
        f.write (content)
        f.write ("\r\n/** @}*/\r\n")


###############################################################################

# Creates a subgroup for a subdir inside a module, and groups all files into the new subgroup
# Works recursively as well to create nested subgroups resembling the filesystem layout

def create_module_subgroup (module_name, module_dir, subdir):

    subgroup_name = subdir.split(os.path.sep)[-1]

    if not subgroup_name:
        raise Excpetion("Error creating module subgroup - empty deduced subgroup name")
        return

    subgroup_definition = []
    subgroup_definition.append ("/** @defgroup {n} {n}".format (n=subgroup_name))
    subgroup_definition.append ("")
    subgroup_definition.append ("    @{")
    subgroup_definition.append ("*/")

    for dirpath, dirnames, filenames in os.walk (os.path.join (module_dir, subdir)):
        for filename in filenames:
            add_doxygen_group (os.path.join (dirpath, filename), subgroup_name)
        for dirname in dirnames:
            subgroup_definition.append ("")
            subgroup_definition.append (create_module_subgroup (module_name, module_dir, os.path.join (subdir, dirname)))

    subgroup_definition.append ("")
    subgroup_definition.append ("/** @} */")

    return "\r\n".join (subgroup_definition)


###############################################################################

# Processes a JUCE module and returns a module description for Doxygen
def process_juce_module (category_name, module_name, module_dir):

    if not category_name:
        raise Exception("Juce module category name cannot be empty!")
        return

    if not module_name:
        raise Exception("Juce module name cannot be empty!")
        return

    if not os.path.isdir (module_dir):
        raise Exception("Error - juce module directory does not exist!")
        return
    
    module_header_info = process_module_header (os.path.join (module_dir, module_name + ".h"))

    module_definiton = []
    module_definiton.append ("/** @defgroup {n} {n}".format (n=module_name))
    module_definiton.append ("    {d}".format (d=module_header_info[0]))
    module_definiton.append ("")
    
    for line in module_header_info[1]:
        module_definiton.append ("    - {l}".format (l=line))

    if module_header_info[2]:
        module_definiton.append ("")
        module_definiton.append ("Preprocessor configuration flags:")

        for preproc_definition in module_header_info[2]:
            module_definiton.append ("    - {l}".format (l=preproc_definition))

    del module_header_info

    module_definiton.append ("")
    module_definiton.append ("    @{")
    module_definiton.append ("*/")

    for item in os.listdir (module_dir):
        if os.path.isdir (os.path.join (module_dir, item)):
            module_definiton.append ("")
            module_definiton.append (create_module_subgroup (module_name, module_dir, item))
        else:
            add_doxygen_group (os.path.join (module_dir, item), module_name)

    module_definiton.append ("")
    module_definiton.append ("/** @} */")

    return "\r\n".join (module_definiton)


###############################################################################

# Processes a text file containing the description of a category of JUCE modules

def process_category_description (file_path):

    if not os.path.exists (file_path):
        raise Exception("Category desription file does not exist!")
        return

    with open (file_path, "r") as f:
        shortDescription = f.readline()
        detailedDescription = f.readline()

    return shortDescription, detailedDescription

###############################################################################

# Processes a group of JUCE modules and returns a module category description for Doxygen

def process_module_category (category_name, orig_cat_dir, dest_cat_dir):

    if not os.path.isdir (orig_cat_dir):
        raise Exception("Module category dir does not exist!")
        return

    if not category_name:
        raise Exception("Module category name cannot be empty!")
        return

    # copy files to doxygen working tree
    shutil.copytree (orig_cat_dir, dest_cat_dir)

    category_info = process_category_description (os.path.join (dest_cat_dir, category_name.lower() + ".txt"))

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


###############################################################################

# Processes the entire heirarchy of juce modules

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

###############################################################################

if __name__ == "__main__":

    parser = ArgumentParser()
    parser.add_argument ("lemons_root", help="the absolute path to the Lemons repo root")
    parser.add_argument ("dest_dir", help="the absolute path to the output")
    
    args = parser.parse_args()

    create_module_heirarchy (args.lemons_root, args.dest_dir)
