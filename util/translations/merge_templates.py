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
				if not stripped_line in lines_to_merge:
						lines_to_merge.append (stripped_line)

	return lines_to_merge


###############################################################################

# Main script

def merge_translation_file (file_to_merge_into, file_to_merge):

	merged_file = remove_duplicates (process_file_for_lines (file_to_merge_into) + process_file_for_lines (file_to_merge))

	merged_file.sort()

	with open (file_to_merge_into, "w") as f:
		f.write ("\n".join (merged_file))
