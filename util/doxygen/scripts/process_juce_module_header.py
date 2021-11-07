#!/usr/bin/env python

import os
import re

###############################################################################


def process_preprocessor_defines (file_contents):

	config_token = "/** Config:"

	preprocessor_defines = []

	while file_contents:
		idx = file_contents.find (config_token)

		if (idx < 0):
			break

		file_contents = file_contents[idx+len(config_token):]

		preprocessor_defines.append (file_contents[:file_contents.find ("*/")])

	return preprocessor_defines


###############################################################################

# Parse information from a JUCE module header's info block.
# Deletes the module header when it's finished, as Doxygen doesn't like them.

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

        if not stripped_line:
        	continue
        
        result = re.match (r"^.*?description:\s*(.*)$", stripped_line)

        if result:
            short_description = result.group (1)
        else:
            detail_lines.append (stripped_line)

    preprocessor_defines = process_preprocessor_defines (file_contents)

    return short_description, detail_lines, preprocessor_defines
