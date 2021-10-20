#!/usr/bin/env python

import os
import re
import argparse


###############################################################################

def process_log_file (input_file, output_file):

	error_lines = []

	with open (input_file, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				if stripped_line.startswith("Error") or stripped_line.startswith("error"):
					error_lines.append (stripped_line)

	num_errors_string = str(len(error_lines)) + " errors found"

	print ("")
	print (num_errors_string)

	with open (output_file, "w") as f:
		f.write (num_errors_string)
		f.write ("")
		f.write ("\r\n\r\n".join (error_lines))


###############################################################################


if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument ("working_dir", help="the working directory to run in")
	parser.add_argument ("log_to_process", help="the input log to process")
	parser.add_argument ("output_file", help="the output log file")
	
	args = parser.parse_args()

	process_log_file (os.path.join (args.working_dir, args.log_to_process),
				      os.path.join (args.working_dir, args.output_file))
