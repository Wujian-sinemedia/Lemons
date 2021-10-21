#!/usr/bin/env python

import os
import re
import argparse


###############################################################################

def process_log_file (input_file, output_file, tokenToSearchFor):

	if not tokenToSearchFor:
		raise Exception("Error - blank token in log post processing!")
		return

	error_lines = []

	with open (input_file, "r") as f:
		for line in f:
			stripped_line = line.strip()
			if stripped_line:
				if tokenToSearchFor.capitalize() in stripped_line or tokenToSearchFor.lower() in stripped_line:
					error_lines.append (stripped_line)

	num_errors = len(error_lines)

	plural_token = tokenToSearchFor.lower() + "s"

	num_errors_string = str(num_errors) + " " + plural_token + " found"

	if num_errors > 0:
		num_errors_string += ":"

	print ("")
	print (num_errors_string)
	for error in error_lines:
		print (error)
	print ("")

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
	parser.add_argument ("token", help="the line token to parse in the input log file")
	
	args = parser.parse_args()
	
	process_log_file (os.path.join (args.working_dir, args.log_to_process),
				      os.path.join (args.working_dir, args.output_file),
				      args.token.strip())
