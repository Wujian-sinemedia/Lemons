#!/usr/bin/env python

import os
import shutil

from argparse import ArgumentParser

###############################################################################

def process_file (file_path, dest_dir):

	root, ext = os.path.splitext (os.path.basename (file_path))

	if not ext == ".cmake":
		return ""

	with open (file_path, "r") as f:
		content = f.read()
		if not content.startswith ("#[["):
			return ""

	content = content[content.find("\n")+1:content.find("]]")]

	output_file = os.path.join (dest_dir, root + ".txt")

	with open (output_file, "w") as f:
		f.write (content)

	return root


###############################################################################


def process_subdir (directory, dest_dir):

	module_names = []

	for dirpath, dirnames, filenames in os.walk (directory):
		for subdir in dirnames:
			subdir_path = os.path.join (dirpath, subdir)
			if os.path.isdir (subdir_path):
				module_names += process_subdir (subdir_path, dest_dir)
		for file in filenames: 
			file_path = os.path.join (dirpath, file)
			if os.path.isfile (file_path):
				module_name = process_file (file_path, dest_dir)
				if module_name:
					module_names.append (module_name)

	return module_names


###############################################################################

def write_doxygen_layout_file (input_file, cmake_module_names, output_file):

	with open(input_file, "r") as f:
		content = f.read()

	for module_name in cmake_module_names:
		pass

	with open(output_file, "w") as f:
		f.write (content)


###############################################################################


if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument ("cmake_modules", help="the absolute path to the CMake modules")
	parser.add_argument ("juce_modules", help="the absolute path to the JUCE modules")
	parser.add_argument ("cmake_doc_output", help="the absolute path to the output")
	parser.add_argument ("doxylayout_input", help="path to the input DoxygenLayout file")
	parser.add_argument ("doxygenlayout_output", help="the path to the DoxygenLayout output file")

	args = parser.parse_args()

	if not os.path.isfile (args.doxylayout_input):
		raise Exception("Input DoxygenLayout.xml file does not exist!")

	if not os.path.isdir (args.cmake_doc_output):
		os.makedirs (args.cmake_doc_output)

	module_names = process_subdir (args.cmake_modules, args.cmake_doc_output)
	module_names += process_subdir (args.juce_modules, args.cmake_doc_output)

	# Remove any duplicates...
	module_names = list(set(module_names))
	module_names.sort()

	write_doxygen_layout_file (args.doxylayout_input, module_names, args.doxygenlayout_output)
