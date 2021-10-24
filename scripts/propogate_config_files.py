#!/usr/bin/env python

# Copies the following files from the Lemons repo to a specified project repo:
# .clang-format
# .clang-tidy

# Also appends the contents of default_gitignore.txt to the repo's .gitignore file.


from shutil import copy2
from os import path
from argparse import ArgumentParser


###############################################################################

# Copies a file from the Lemons repo root to the specified output directory

def copy_file_verbatim (filename, lemons_root, dest_repo):

	orig_path = path.join (lemons_root, filename)

	if not path.exists (orig_path):
		raise Exception("Non-existant input file!")
		return

	dest_path = path.join (dest_repo, filename)

	copy2 (orig_path, dest_path)


###############################################################################

# Appends elements from the default .gitignore to the dest gitignore

def process_gitignore (default_gitignore, dest_repo):

	dest_file = path.join (dest_repo, ".gitignore")

	if path.exists (dest_file):
		with open (dest_file, "r") as f:
			prev_lines = f.read()
	else:
		prev_lines = ""

	with open (default_gitignore, "r") as default:
		with open (dest_file, "a") as dest:
			for line in default:
				if not line in prev_lines:
					dest.write ("\n" + line)


###############################################################################


if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("output_dir", help="the project repository to copy files to")
	
	args = parser.parse_args()

	if not path.isdir (args.output_dir):
		raise Exception("Non-existant destination repo!")

	script_dir = path.abspath (path.dirname (__file__))

	lemons_root = path.abspath (path.dirname (script_dir))

	copy_file_verbatim (".clang-format", lemons_root, args.output_dir)

	copy_file_verbatim (".clang-tidy", lemons_root, args.output_dir)

	default_gitignore = path.join (script_dir, "default_gitignore.txt")

	if not path.exists (default_gitignore):
		raise Exception("Non-existant source default gitignore!")

	process_gitignore (default_gitignore, args.output_dir)

	process_gitignore (default_gitignore, lemons_root)  # Also make sure the Lemons gitignore itself is in sync with the default one
