#!/usr/bin/env python

import os
import re

#

def process_preprocessor_defines (file_contents):

	config_token = "/** Config:"

	preprocessor_defines = []

	while file_contents:
		idx = file_contents.find (config_token)

		if (idx < 0): break

		file_contents = file_contents[idx+len(config_token):]

		preprocessor_defines.append (file_contents[:file_contents.find ("*/")])

	return preprocessor_defines

#

def process_module_header (header_path):

    if not os.path.exists (header_path):
        raise Exception("Cannot parse nonexistent module header!")
        return

    with open (header_path, "r") as f:
        file_contents = f.read()

    os.remove (header_path) # The module header causes problems for Doxygen, so delete it

    block_info_result = re.match (r".*BEGIN_JUCE_MODULE_DECLARATION"
                                      "(.*)"
                                      "END_JUCE_MODULE_DECLARATION.*",
                                      file_contents,
                                      re.DOTALL)

    detail_lines = []

    for line in block_info_result.group (1).split ("\n"):

        stripped_line = line.strip()

        if not stripped_line: continue
        
        result = re.match (r"^.*?description:\s*(.*)$", stripped_line)

        if result:
            short_description = result.group (1)
            continue

        deps_token = "dependencies:"

        idx = line.find (deps_token)

        if idx < 0:
            detail_lines.append (stripped_line)
            continue

        new_line = deps_token

        for dep in line[idx:].split():
            if dep == deps_token: continue
            new_line += " [{n}](@ref {n})".format(n=dep)
            
        detail_lines.append (new_line)

    preprocessor_defines = process_preprocessor_defines (file_contents)

    return short_description, detail_lines, preprocessor_defines

#

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

#

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

#

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
