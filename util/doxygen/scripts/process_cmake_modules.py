#!/usr/bin/env python

import os

from argparse import ArgumentParser

from process_cmake_module import process_file

###############################################################################

def get_output_file (dest_dir):
	return os.path.join (dest_dir, "CMakeModules.md")


def add_category_to_output_file (dest_dir, category):

	if category == "scripts" or category == "modules":
		return

	dest_file = get_output_file (dest_dir)

	formatted_line = "### {c}".format (c=category.capitalize())

	if not os.path.isfile (dest_file):
		with open (dest_file, "a") as f:
			f.write (formatted_line)
		return

	with open (dest_file, "r") as f:
		for line in f:
			if line.startswith (formatted_line):
				return

	with open (dest_file, "a") as f:
		f.write ("\r\n")
		f.write (formatted_line)


###############################################################################

def process_files_in_subdir (dirpath, dest_dir, category):

	if not os.path.isdir (dest_dir):
		os.makedirs (dest_dir)

	dest_file = get_output_file (dest_dir)

	add_category_to_output_file (dest_dir, category)

	for path, dirnames, filenames in os.walk (dirpath):
		for file in filenames:
			file_path = os.path.join (dirpath, file)
			if os.path.isfile (file_path):
				process_file (file_path, dest_dir, dest_file, category)

	if os.path.isfile (dest_file):
		with open (dest_file, "a") as f:
			f.write ("\r\n")

#

def process_subdir (directory, dest_dir, category):

	if category:
		flatten = True
	else:
		flatten = False
		category = os.path.basename (directory)

	process_files_in_subdir (directory, dest_dir, category)

	for dirpath, dirnames, filenames in os.walk (directory):
		for subdir in dirnames:
			subdir_path = os.path.join (dirpath, subdir)

			if os.path.isdir (subdir_path):
				if flatten:
					rec_cat = category
				else:
					rec_cat = os.path.basename (subdir_path)
					
				process_files_in_subdir (subdir_path, dest_dir, rec_cat)

###############################################################################

if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument ("cmake_modules", help="the absolute path to the CMake modules")
	parser.add_argument ("juce_modules", help="the absolute path to the JUCE modules")
	parser.add_argument ("cmake_doc_output", help="the absolute path to the output")
	parser.add_argument ("cmake_api_input", help="path to the CMake API input file")

	args = parser.parse_args()

	dest_file = get_output_file (args.cmake_doc_output)
	if os.path.isfile (dest_file): os.remove (dest_file)

	process_subdir (args.cmake_modules, args.cmake_doc_output, "")
	process_subdir (args.juce_modules, args.cmake_doc_output, "JUCE module inclusion")

	with open (dest_file, "r") as f:
		module_list = f.read()

	with open (args.cmake_api_input, "r") as f:
		cmake_api_input = f.read()

	with open (dest_file, "w") as f:
		f.write (cmake_api_input)
		f.write ("\r\n\r\n")
		f.write ("## CMake modules")
		f.write ("\r\n")
		f.write (module_list)
