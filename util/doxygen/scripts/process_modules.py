#!/usr/bin/env python

import os
import shutil
import re

###############################################################################

# Add a Doxygen group to the file at 'path'

def add_doxygen_group (path, group_name):

    if not os.path.exists (path):
        raise Exception("Cannot add Doxygen group to nonexistent filepath!")
        return

    if not group_name:
        raise Exception("Doxygen group name cannot be empty!")
        return

    with open (path, "r") as f:
        content = f.read()

    with open (path, "w") as f:
        f.write ("\r\n/** @ingroup " + group_name + "\r\n *  @{\r\n */\r\n")
        f.write (content)
        f.write ("\r\n/** @}*/\r\n")


###############################################################################

# Parse information from a JUCE module header's info block.
# Deletes the module header when it's finished, as Doxygen doesn't like them.

def process_module_header (header_path):

    if not os.path.exists (header_path):
        raise Exception("Cannot parse nonexistent module header!")
        return

    with open (header_path, "r") as f:
        block_info_result = re.match (r".*BEGIN_JUCE_MODULE_DECLARATION"
                                      "(.*)"
                                      "END_JUCE_MODULE_DECLARATION.*",
                                      f.read(),
                                      re.DOTALL)

    os.remove (header_path) # The module header causes problems for Doxygen, so delete it

    detail_lines = []

    for line in block_info_result.group (1).split ("\n"):

        stripped_line = line.strip()
        
        if stripped_line:
            result = re.match (r"^.*?description:\s*(.*)$", stripped_line)
            if result:
                short_description = result.group (1)
            else:
                detail_lines.append (stripped_line)

    return short_description, detail_lines


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
        content = f.read()

    return content

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

    category_definiton = []
    category_definiton.append ("/** @defgroup {n} {n}".format (n=category_name))
    category_definiton.append ("    {d}".format (d=process_category_description (os.path.join (dest_cat_dir, 
                                                                                 category_name.lower() + ".txt"))))
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
