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
#  ======================================================================================

import os
import shutil
from argparse import ArgumentParser

#


def get_category_description_from_cmake_module(dir_path):
	# pylint: disable-next=unused-variable
	for dirpath, dirnames, filenames in os.walk(dir_path):
		for file in filenames:
			root, ext = os.path.splitext(file)
			if ext == ".cmake":
				return root

	return ""


#


def process_category_description(dir_path):

	category_cmake_module = get_category_description_from_cmake_module(
	    dir_path)

	if not category_cmake_module:
		raise Exception(
		    "Can't find CMake module in this JUCE module category directory!")

	description = ""

	with open(os.path.join(dir_path, category_cmake_module + ".cmake"),
	          "r") as f:
		for line in f:
			if line.startswith("#[["):
				continue
			description = line.strip()
			break

	if not description:
		raise Exception("Description could not be parsed!")

	cmakeInfo = "To use these JUCE modules, include the [{n}](@ref {n}) CMake module, and link against the `{n}` target.".format(
	    n=category_cmake_module)

	return description, cmakeInfo


#


def process_module_category(category_name, orig_cat_dir, dest_cat_dir):

	if not os.path.isdir(orig_cat_dir):
		raise Exception("Module category dir does not exist!")

	if not category_name:
		raise Exception("Module category name cannot be empty!")

	if os.path.isdir(dest_cat_dir):
		shutil.rmtree(dest_cat_dir)

	shutil.copytree(orig_cat_dir, dest_cat_dir)

	category_info = process_category_description(dest_cat_dir)

	category_definition = []
	category_definition.append("/** @defgroup {n} {n}".format(n=category_name))

	category_definition.append("")
	category_definition.append(f"    {category_info[0]}")
	category_definition.append("")
	category_definition.append(f"    {category_info[1]}")

	del category_info

	category_definition.append("*/")
	category_definition.append("")

	return "\r\n".join(category_definition)


#


def create_module_hierarchy(source_dir, dest_dir):

	if not os.path.isdir(source_dir):
		raise Exception("Juce modules source directory does not exist!")

	orig_module_dir = os.path.join(source_dir, "modules")

	if not os.path.isdir(orig_module_dir):
		raise Exception("Juce modules source directory does not exist!")

	category_definitions = []

	for category in os.listdir(orig_module_dir):
		category_path = os.path.join(orig_module_dir, category)
		if os.path.isdir(category_path):
			category_definitions.append(
			    process_module_category(category, category_path,
			                            os.path.join(dest_dir, category)))

	# Create a .dox file containing the entire module hierarchy
	with open(os.path.join(dest_dir, "lemons_modules.dox"), "w") as f:
		f.write("\r\n\r\n".join(category_definitions))


#

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument("lemons_root",
	                    help="the absolute path to the Lemons repo root")
	parser.add_argument("dest_dir", help="the absolute path to the output")

	args = parser.parse_args()

	create_module_hierarchy(args.lemons_root, args.dest_dir)
