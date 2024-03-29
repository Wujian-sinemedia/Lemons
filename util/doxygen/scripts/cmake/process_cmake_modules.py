#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

import os
from argparse import ArgumentParser

from cmake_api import process_cmake_api
from process_cmake_module import process_file

#


def get_output_file(dest_dir):
	return os.path.join(dest_dir, "CMakeModules.md")


def add_category_to_output_file(output_file, category):

	if category in ("scripts", "modules"):
		return

	formatted_line = f"### {category.capitalize()}"

	if not os.path.isfile(output_file):
		with open(output_file, "a") as out_file:
			out_file.write(formatted_line)
		return

	with open(output_file, "r") as file:
		for line in file:
			if line.startswith(formatted_line):
				return

	with open(output_file, "a") as out_file:
		out_file.write("\r\n")
		out_file.write(formatted_line)


#


def process_files_in_subdir(dirpath, dest_dir, category):

	destination_file = get_output_file(dest_dir)

	add_category_to_output_file(destination_file, category)

	# pylint: disable-next=unused-variable
	for path, dirnames, filenames in os.walk(dirpath):
		for file in filenames:
			file_path = os.path.join(dirpath, file)
			if os.path.isfile(file_path):
				process_file(file_path, dest_dir, destination_file, category)

	if os.path.isfile(destination_file):
		with open(destination_file, "a") as file:
			file.write("\r\n")


#


def process_subdir(directory, dest_dir, category):

	if category:
		flatten = True
	else:
		flatten = False
		category = os.path.basename(directory)

	process_files_in_subdir(directory, dest_dir, category)

	# pylint: disable-next=unused-variable
	for dirpath, dirnames, filenames in os.walk(directory):
		for subdir in dirnames:
			subdir_path = os.path.join(dirpath, subdir)

			if not os.path.isdir(subdir_path):
				continue

			if flatten:
				rec_cat = category
			else:
				rec_cat = os.path.basename(subdir_path)

			process_files_in_subdir(subdir_path, dest_dir, rec_cat)


#

if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument("cmake_modules",
	                    help="the absolute path to the CMake modules")
	parser.add_argument("juce_modules",
	                    help="the absolute path to the JUCE modules")
	parser.add_argument("lemons_root",
	                    help="the absolute path to the Lemons repo root")
	parser.add_argument("cmake_doc_output",
	                    help="the absolute path to the output")
	parser.add_argument("cmake_api_input",
	                    help="path to the CMake API input file")

	args = parser.parse_args()

	dest_file = get_output_file(args.cmake_doc_output)
	if os.path.isfile(dest_file):
		os.remove(dest_file)

	if not os.path.isdir(args.cmake_doc_output):
		os.makedirs(args.cmake_doc_output)

	process_subdir(args.cmake_modules, args.cmake_doc_output, "")
	process_subdir(args.juce_modules, args.cmake_doc_output,
	               "JUCE module inclusion")

	with open(dest_file, "r") as f:
		module_list = f.read()

	cmake_api_text = process_cmake_api(args.cmake_api_input, args.lemons_root)

	with open(dest_file, "w") as f:
		f.write(cmake_api_text)
		f.write("\r\n")
		f.write(module_list)
