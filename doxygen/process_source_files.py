#!/usr/bin/env python

import os
import shutil
import re


###############################################################################

# Add a Doxygen group to the file at 'path'

def add_doxygen_group (path, group_name):

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

    with open (header_path, "r") as f:
        block_info_result = re.match (r".*BEGIN_JUCE_MODULE_DECLARATION"
                                      "(.*)"
                                      "END_JUCE_MODULE_DECLARATION.*",
                                      f.read(),
                                      re.DOTALL)

    # The module header causes problems for Doxygen, so delete it
    os.remove (header_path)

    detail_lines = []

    # read lines from the module description block
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

def create_module_subgroup (module_name, module_dir, subdir):

    subgroup_name = "{n}-{s}".format (n=module_name, s=subdir)

    for dirpath, dirnames, filenames in os.walk (os.path.join (module_dir, subdir)):
        for filename in filenames:
            add_doxygen_group (os.path.join (dirpath, filename), subgroup_name)

    return "/** @defgroup {tag} {n} */".format (tag=subgroup_name, n=subdir)


###############################################################################

# Processes a JUCE module and returns a module description for Doxygen
def process_juce_module (category_name, module_name, module_dir):
    
    module_header_info = process_module_header (os.path.join (module_dir, module_name + ".h"))

    # Create a Doxygen group definition for the module
    module_definiton = []
    module_definiton.append ("/** @defgroup {n} {n}".format (n=module_name))
    module_definiton.append ("    {d}".format (d=module_header_info[0]))
    module_definiton.append ("")
    
    for line in module_header_info[1]:
        module_definiton.append ("    - {l}".format (l=line))

    module_definiton.append ("")
    module_definiton.append ("    @{")
    module_definiton.append ("*/")

    # put the module's contents into appropriate doxygen subgroups
    for item in os.listdir (module_dir):
        if (os.path.isdir (os.path.join (module_dir, item))):
            # subdirectory within module
            subgroup_definition = create_module_subgroup (module_name, module_dir, item)
            module_definiton.append ("")
            module_definiton.append (subgroup_definition)
        else:
            # top-level files within module
            add_doxygen_group (os.path.join (module_dir, item), module_name)

    module_definiton.append ("")
    module_definiton.append ("/** @} */")

    return module_definiton


###############################################################################

# Processes a group of JUCE modules and returns a module category description for Doxygen

def process_module_category (category_name, orig_cat_dir, dest_cat_dir):

    # copy files to doxygen working tree
    shutil.copytree (orig_cat_dir, dest_cat_dir)

    # Create a Doxygen group definition for the category
    category_definiton = []
    category_definiton.append ("/** @defgroup {n} {n}".format (n=category_name))

    category_definiton.append ("")
    category_definiton.append ("    @{")
    category_definiton.append ("*/")

    # process all the modules in this category
    for subdir in os.listdir  (dest_cat_dir):
        module_path = os.path.join (dest_cat_dir, subdir)

        if os.path.isdir (module_path):
            module_definition = process_juce_module (category_name, subdir, module_path)
            category_definiton.append ("\r\n".join (module_definition))

    category_definiton.append ("")
    category_definiton.append ("/** @} */")

    # create a header file for the category
    with open (os.path.join (dest_cat_dir, category_name + ".h"), "w") as f:
        f.write ("\r\n\r\n".join (category_definiton))

    return category_definiton


###############################################################################

# Processes the entire heirarchy of juce modules

def process_juce_modules (source_dir, dest_dir):

    orig_module_dir = os.path.join (source_dir, "modules")

    category_definitions = []

    for category in os.listdir (orig_module_dir):
        if os.path.isdir (os.path.join (orig_module_dir, category)):
            category_definition = process_module_category (category,
                                                           os.path.join (orig_module_dir, category),
                                                           os.path.join (dest_dir, category))

            category_definitions.append ("\r\n".join (category_definition))

    # Create a .dox file containing the entire module hierarchy
    with open (os.path.join (dest_dir, "lemons_modules.dox"), "w") as f:
        f.write ("\r\n\r\n".join (category_definitions))


###############################################################################

# Copies the readme from the cmake directory into the working tree for doxygen

def copy_cmake_readme (source_dir, dest_dir):

    dest = os.path.join (dest_dir, "cmake_api.md")

    # create an empty file
    with open (dest, "w") as f:
        pass

    shutil.copy2 (os.path.join (os.path.join (source_dir, "cmake"), "README.md"), 
                  dest)


###############################################################################

# Main script 

if __name__ == "__main__":

    script_dir = os.path.abspath (os.path.dirname (__file__))

    source_dir = os.path.abspath (os.path.dirname (script_dir))

    dest_dir = os.path.join (script_dir, "build")

    # delete the working dir if it exists
    if os.path.isdir (dest_dir):
        shutil.rmtree (dest_dir)

    # re-create a clean working directory
    os.mkdir (dest_dir)

    # copy cmake API readme to the build tree
    copy_cmake_readme (source_dir, dest_dir)

    # process juce modules
    process_juce_modules (source_dir, dest_dir)
