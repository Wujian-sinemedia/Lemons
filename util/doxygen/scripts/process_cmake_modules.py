#!/usr/bin/env python

import os
import shutil

from argparse import ArgumentParser

###############################################################################

def process_file (file_path, dest_dir, category):

	root, ext = os.path.splitext (os.path.basename (file_path))

	if not ext == ".cmake": return

	with open (file_path, "r") as f:
		content = f.read()
	
	if not content.startswith ("#[["): return

	content = content[content.find("\n")+1:content.find("]]")]

	output_file = os.path.join (dest_dir, root + ".md")

	with open (output_file, "w") as f:
		f.write (content)

#

def process_files_in_subdir (dirpath, dest_dir, category):
	for path, dirnames, filenames in os.walk (dirpath):
		for file in filenames:
			file_path = os.path.join (dirpath, file)
			if os.path.isfile (file_path):
				process_file (file_path, dest_dir, category)

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

	args = parser.parse_args()

	if not os.path.isdir (args.cmake_doc_output):
		os.makedirs (args.cmake_doc_output)

	process_subdir (args.cmake_modules, args.cmake_doc_output, "")
	process_subdir (args.juce_modules, args.cmake_doc_output, "JUCE module inclusion")
