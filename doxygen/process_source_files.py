#!/usr/bin/env python

import os
import shutil
import re


# Add a Doxygen group to the file at 'path'
def add_doxygen_group(path, group_name):

    with open(path, "r") as f:
        content = f.read()

    with open(path, "w") as f:
        f.write("\r\n/** @weakgroup " + group_name + "\r\n *  @{\r\n */\r\n")
        f.write(content)
        f.write("\r\n/** @}*/\r\n")

###############################################################################

# Parse information from a JUCE module header's info block.
# Deletes the module header when it's finished, as Doxygen doesn't like them.
def process_module_header(header_path):

    with open(header_path, "r") as f:
        content = f.read()

    block_info_result = re.match(r".*BEGIN_JUCE_MODULE_DECLARATION"
                                 "(.*)"
                                 "END_JUCE_MODULE_DECLARATION.*",
                                 content,
                                 re.DOTALL)

    detail_lines = []

    for line in block_info_result.group(1).split("\n"):
        stripped_line = line.strip()
        
        if stripped_line:
            result = re.match(r"^.*?description:\s*(.*)$", stripped_line)
            if result:
                short_description = result.group(1)
            else:
                detail_lines.append(stripped_line)

    # The module header causes problems for Doxygen, so delete it
    os.remove(header_path)

    return short_description, detail_lines

###############################################################################

# Processes a JUCE module and returns a module description for Doxygen
def process_juce_module(module_name, module_dir):
    
    module_header_info = process_module_header(os.path.join(module_dir, module_name + ".h"))

    # Create a Doxygen group definition for the module
    module_definiton = []
    module_definiton.append("/** @defgroup {n} {n}".format(n=module_name))
    module_definiton.append("    {d}".format(d=module_header_info[0]))
    
    module_definiton.append("")
    for line in module_header_info[1]:
        module_definiton.append("    - {l}".format(l=line))

    module_definiton.append("")
    module_definiton.append("    @{")
    module_definiton.append("*/")

    # Create a list of the directories in the module that we can use as subgroups and create the Doxygen group hierarchy string
    dir_contents = os.listdir(module_dir)

    # Ignore "native" folders
    try:
        dir_contents.remove("native")
    except ValueError:
        pass

    subdirs = []
    for item in dir_contents:
        if (os.path.isdir(os.path.join(module_dir, item))):
            subdirs.append(item)

    module_groups = {}
    for subdir in subdirs:
        subgroup_name = "{n}-{s}".format(n=module_name, s=subdir)
        module_groups[subgroup_name] = os.path.join(module_dir, subdir)
        module_definiton.append("")
        module_definiton.append("/** @defgroup {tag} {n} */".format(tag=subgroup_name, n=subdir))
        
    module_definiton.append("")
    module_definiton.append("/** @} */")

    # Put the top level files into the main group
    for filename in (set(dir_contents) - set(subdirs)):
        add_doxygen_group(os.path.join(module_dir, filename), 
                          module_name)

    # Put subdirectory files into their respective groups
    for group_name in module_groups:
        for dirpath, dirnames, filenames in os.walk(module_groups[group_name]):
            for filename in filenames:
                add_doxygen_group(os.path.join(dirpath, filename), 
                                  group_name)

    return module_definiton

###############################################################################

# Processes a group of JUCE modules and returns a module category description for Doxygen
def process_module_category(category_name, orig_cat_dir, dest_cat_dir):

    # copy files to doxygen working tree
    shutil.copytree(orig_cat_dir, dest_cat_dir)

    # Create a Doxygen group definition for the category
    category_definiton = []
    category_definiton.append("/** @defgroup {n} {n}".format(n=category_name))

    category_definiton.append("")
    category_definiton.append("    @{")
    category_definiton.append("*/")

    modules = {}

    for subdir in os.listdir(dest_cat_dir):
        if os.path.isdir(os.path.join(dest_cat_dir, subdir)):
            modules[subdir] = os.path.join(dest_cat_dir, subdir)
            module_definition = process_juce_module(subdir, os.path.join(dest_cat_dir, subdir))
            category_definiton.append("\r\n".join(module_definition))

    category_definiton.append("")
    category_definiton.append("/** @} */")

    return category_definiton

###############################################################################

# Copies the readme from the cmake directory into the working tree for doxygen
def copy_cmake_readme(source_dir, dest_dir):
    dest = os.path.join(dest_dir, "cmake_api.md")

    # create an empty file
    with open(dest, "w") as f:
        pass

    src = os.path.join(source_dir, "cmake")
    src = os.path.join(src, "README.md")
    shutil.copy2(src, dest)

###############################################################################

# Main script 
if __name__ == "__main__":

    script_dir = os.path.abspath(os.path.dirname(__file__))

    source_dir = os.path.abspath(os.path.dirname(script_dir))

    dest_dir = os.path.join(script_dir, "build")

    # delete the working dir if it exists
    try:
        shutil.rmtree(dest_dir)
    except OSError:
        pass
    except FileNotFoundError:
        pass

    # re-create a clean working directory
    os.mkdir(dest_dir)

    # copy cmake API readme to build tree
    copy_cmake_readme(source_dir, dest_dir)

    # process juce modules

    orig_module_dir = os.path.join(source_dir, "modules")

    category_definitions = []

    for category in os.listdir(orig_module_dir):
        if os.path.isdir(os.path.join(orig_module_dir, category)):
            category_definition = process_module_category(category,
                                                          os.path.join(orig_module_dir, category),
                                                          os.path.join(dest_dir, category))

            category_definitions.append("\r\n".join(category_definition))

    
    # Create an extra header file containing the module hierarchy
    with open(os.path.join(dest_dir, "lemons_modules.dox"), "w") as f:
        f.write("\r\n\r\n".join(category_definitions))
