#!/usr/bin/env python

import os
import shutil

from argparse import ArgumentParser

###############################################################################

def process_file (file_path, dest_dir):

	root, ext = os.path.splitext (file_path)

	if not ext == ".cmake":
		return

	with open (file_path, "r") as f:
		content = f.read()
		if not content.startswith ("#[["):
			return

	content = content[content.find("\n")+1:content.find("]]")]

	root, ext = os.path.splitext (os.path.basename (file_path))
	output_file = os.path.join (dest_dir, root + ".txt")

	with open (output_file, "w") as f:
		f.write (content)


###############################################################################


def process_subdir (directory, dest_dir):
	for dirpath, dirnames, filenames in os.walk (directory):
		for subdir in dirnames:
			subdir_path = os.path.join (dirpath, subdir)
			if os.path.isdir (subdir_path):
				process_subdir (subdir_path, dest_dir)
		for file in filenames: 
			file_path = os.path.join (dirpath, file)
			if os.path.isfile (file_path):
				process_file (file_path, dest_dir)


###############################################################################


if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument ("cmake_modules", help="the absolute path to the CMake modules")
	parser.add_argument ("juce_modules", help="the absolute path to the JUCE modules")
	parser.add_argument ("dest_dir", help="the absolute path to the output")

	args = parser.parse_args()

	if not os.path.isdir (args.dest_dir):
		os.makedirs (args.dest_dir)

	process_subdir (args.cmake_modules, args.dest_dir)
	process_subdir (args.juce_modules, args.dest_dir)
