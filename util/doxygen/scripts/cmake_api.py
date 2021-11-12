#!/usr/bin/env python

import os

#

def split_args_from_line (line):

	line = line[line.find("(")+1:line.find(")")-1]

	args = []

	inMultiwordArg = False
	currentArg = ""

	for word in line.split():
		if word == "\"\"":
			args.append (" ")
			continue

		if word.startswith ('"'):
			inMultiwordArg = True
			currentArg = word[1:]
		elif word.endswith ('"'):
			if inMultiwordArg:
				inMultiwordArg = False
				currentArg += word[:-1]
				args.append (currentArg)
				currentArg = ""
			else:
				args.append (word)
		else:
			if inMultiwordArg:
				currentArg += word
			else:
				args.append (word)

	return args

#

def parse_option_from_line (line):
	args = split_args_from_line (line)

	return "*{n}:* {t}. Defaults to {d}.".format(n=args[0], t=args[1].strip('"'), d=args[2])


def parse_dependant_option_from_line (line):
	args = split_args_from_line (line)

	return "*{n}:* {t}".format(n=args[0], t=args[1].strip('"'))


def get_options_from_cmakelists (orig_text):
	options = []

	for line in orig_text.splitlines():
		stripped_line = line.strip()
		if not stripped_line: continue

		if stripped_line.startswith ("option"):
			options.append (parse_option_from_line (stripped_line))
		elif stripped_line.startswith ("cmake_dependent_option"):
			options.append (parse_dependant_option_from_line (stripped_line))

	return options

#

def parse_cache_var_from_line (line):
	args = split_args_from_line (line)

	print ("Num args: " + str(len(args)))
	print ("name: " + args[0])

	return "*{n}:* {t}. Defaults to {d}.".format(n=args[0], t=args[4].strip('"'), d=args[1].strip('"'))


def get_cache_vars_from_cmakelists (orig_text):
	cache_vars = []

	for line in orig_text.splitlines():
		stripped_line = line.strip()
		if not stripped_line: continue

		if stripped_line.startswith ("set"):
			if "CACHE" in line:
				cache_vars.append (parse_cache_var_from_line (stripped_line))

	return cache_vars

#

def process_cmake_api (cmake_api_input, lemons_root):

	lemons_cmakelists = os.path.join (lemons_root, "CMakeLists.txt")

	if not os.path.isfile (lemons_cmakelists):
		raise Exception("Lemons CMakeLists.txt cannot be found!")
		return

	with open (lemons_cmakelists, "r") as f:
		cmakelists_text = f.read()

	output_text = "# Lemons CMake API		{#CMake_API}"

	output_text += "\r\n ## CMake options \r\n"
	output_text += "\r\n".join (get_options_from_cmakelists (cmakelists_text))
	output_text += "\r\n".join (get_cache_vars_from_cmakelists (cmakelists_text))
	output_text += "\r\n\r\n --- \r\n\r\n"

	with open (cmake_api_input, "r") as f:
		output_text += f.read()

	return output_text
