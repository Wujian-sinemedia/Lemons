#!/usr/bin/env python

import os
import shutil
import re
import argparse


# Add a Doxygen group to the file at 'path'
def add_doxygen_group(path, group_name):
    filename = os.path.basename(path)

    if re.match(r"^lemons_.*\.(h|dox)", filename):
        with open(path, "r") as f:
            content = f.read()
        with open(path, "w") as f:
            f.write("\r\n/** @weakgroup " + group_name + "\r\n *  @{\r\n */\r\n")
            f.write(content)
            f.write("\r\n/** @}*/\r\n")


###############################################################################


def process_juce_module(module_name, original_module_dir, dest_module_dir):
    
    shutil.copytree(original_module_dir, dest_module_dir)

    # Parse the module header to get module information
    module_header = os.path.join(dest_module_dir, module_name + ".h")
    with open(module_header, "r") as f:
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
    os.remove(module_header)

    # Create a Doxygen group definition for the module
    module_definiton = []
    module_definiton.append("/** @defgroup {n} {n}".format(n=module_name))
    module_definiton.append("    {d}".format(d=short_description))
    module_definiton.append("")
    for line in detail_lines:
        module_definiton.append("    - {l}".format(l=line))
    module_definiton.append("")
    module_definiton.append("    @{")
    module_definiton.append("*/")

    # Create a list of the directories in the module that we can use as subgroups and create the Doxygen group hierarchy string
    dir_contents = os.listdir(dest_module_dir)

    # Ignore "native" folders
    try:
        dir_contents.remove("native")
    except ValueError:
        pass

    subdirs = []
    for item in dir_contents:
        if (os.path.isdir(os.path.join(dest_module_dir, item))):
            subdirs.append(item)

    module_groups = {}
    for subdir in subdirs:
        subgroup_name = "{n}-{s}".format(n=module_name, s=subdir)
        module_groups[subgroup_name] = os.path.join(dest_module_dir, subdir)
        module_definiton.append("")
        module_definiton.append("/** @defgroup {tag} {n} */".format(tag=subgroup_name, n=subdir))
        
    module_definiton.append("")
    module_definiton.append("/** @} */")

    # Put the top level files into the main group
    for filename in (set(dir_contents) - set(subdirs)):
        add_doxygen_group(os.path.join(dest_module_dir, filename), module_name)

    # Put subdirectory files into their respective groups
    for group_name in module_groups:
        for dirpath, dirnames, filenames in os.walk(module_groups[group_name]):
            for filename in filenames:
                filepath = os.path.join(dirpath, filename)
                add_doxygen_group(filepath, group_name)

    return module_definiton


###############################################################################


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("source_dir",
                        help="the directory to search for source files")
    parser.add_argument("dest_dir",
                        help="the directory in which to place processed files")
    args = parser.parse_args()

    try:
        shutil.rmtree(args.dest_dir)
    except OSError:
        pass
    except FileNotFoundError:
        pass

    # create the list of juce modules to include
    juce_modules = []
    for category in os.listdir(args.source_dir):
        cur_subdir = os.path.join(args.source_dir, category)
        if os.path.isdir(cur_subdir):
            for module in os.listdir(cur_subdir):
                if os.path.isdir(os.path.join(cur_subdir, module)):
                    juce_modules.append(category + os.path.sep + module)

    # Copy the juce modules to the temporary directory, and process the source files
    module_definitions = []
    for module_path in juce_modules:

        module_name = module_path.split(os.path.sep, 1)[-1]
        original_module_dir = os.path.join(args.source_dir, module_path)
        dest_module_dir = os.path.join(args.dest_dir, module_path)

        module_definiton = process_juce_module(module_name, original_module_dir, dest_module_dir)

        module_definitions.append("\r\n".join(module_definiton))

    # Create an extra header file containing the module hierarchy
    with open(os.path.join(args.dest_dir, "lemons_modules.dox"), "w") as f:
        f.write("\r\n\r\n".join(module_definitions))
