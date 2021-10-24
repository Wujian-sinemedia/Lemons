#!/usr/bin/env python

import os
from argparse import ArgumentParser


def process_dir (dir_path):

	if not os.path.isdir (dir_path):
		return

	os.system ("clang-format -i *.h *.hpp *.c *.cpp")

	for subdir in os.listdir (dir_path):
		process_dir (os.path.join (dir_path, subdir))


#


if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("directory", help="the directory to search for source files")
	args = parser.parse_args()

	process_dir (args.directory)