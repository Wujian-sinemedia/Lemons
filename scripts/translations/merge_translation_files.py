#!/usr/bin/env python

from argparse import ArgumentParser


###############################################################################

def remove_duplicates (orig_list):
	new_list = []

	for item in orig_list:
		if item not in new_list:
			new_list.append (item)

	return new_list


def process_file_for_lines (file_path):

	lines_to_merge = []

	with open (file_path, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				if stripped_line.startswith ("\""):
					if not stripped_line in lines_to_merge:
						lines_to_merge.append (stripped_line)

	return lines_to_merge


def merge_translation_file (io_file, fileToMerge):

	merged_file = remove_duplicates (process_file_for_lines (io_file) + process_file_for_lines (fileToMerge))

	with open (io_file, "w") as f:
		f.write ("language: \n")
		f.write ("countries: \n\n")
		f.write ("\n".join (merged_file))


###############################################################################

# Main script

if __name__ == "__main__":

	parser = ArgumentParser()
	parser.add_argument ("file_to_merge_into", help="the file to merge into")
	parser.add_argument ("-l", "--list", nargs='+', dest='files', help="the list of files to merge into the main file")

	args = parser.parse_args()

	for file in args.files:
		merge_translation_file (args.file_to_merge_into, file)
