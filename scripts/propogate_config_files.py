#!/usr/bin/env python

# Copies the following files from the Lemons repo to a specified project repo:
# .gitignore
# .clang-format
# .clang-tidy


import argparse


###############################################################################


if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument ("output_dir", help="the project repository to copy files to")
	
	args = parser.parse_args()

	script_dir = os.path.abspath (os.path.dirname (__file__))