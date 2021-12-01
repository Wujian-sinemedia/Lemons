#!/usr/bin/env python

import os

#

def process_module_includes (orig_content):
	output_lines = []
	inIncludesSection = False

	for line in orig_content.splitlines():
		if line.startswith ("## Includes:"):
			inIncludesSection = True
			output_lines.append (line)
			continue
		elif inIncludesSection:
			if not line.startswith ("-"):
				inIncludesSection = False

		if inIncludesSection:
			include = line[1:].strip()
			line = "- [{n}](@ref {n})".format(n=include)

		output_lines.append (line)

	return "\r\n".join(output_lines)

#

def get_list_of_juce_modules_in_category (cmake_module_path):
	print ("Processing juce modules for module: " + cmake_module_path)

	modules = []

	category_dir = os.path.abspath (os.path.dirname (cmake_module_path))

	for item in os.listdir (category_dir):
		if os.path.isdir (os.path.join (category_dir, item)):
			modules.append (item)

	return modules


def process_module_targets (orig_content, cmake_module_path):
	output_lines = []
	inTargetsSection = False
	wroteTargets = False

	for line in orig_content.splitlines():
		if line.startswith ("## Targets:"):
			inTargetsSection = True
			output_lines.append (line)
			continue
		elif inTargetsSection:
			if not line.startswith ("-"):
				inTargetsSection = False
				wroteTargets = True
				for module in get_list_of_juce_modules_in_category (cmake_module_path):
					output_lines.append ("- [{n}](@ref {n})".format(n=module))
		
		output_lines.append (line)

	if not wroteTargets:
		for module in get_list_of_juce_modules_in_category (cmake_module_path):
					output_lines.append ("- [{n}](@ref {n})".format(n=module))

	return "\r\n".join(output_lines)

#

def add_module_to_output_file (dest_dir, module, dest_file):
	with open (dest_file, "a") as f:
		f.write ("\r\n")
		f.write ("- [{n}](@ref {n})".format(n=module))

#

def process_file (file_path, dest_dir, dest_file, category):

	name, ext = os.path.splitext (os.path.basename (file_path))

	if not ext == ".cmake": return

	with open (file_path, "r") as f:
		content = f.read()
	
	if not content.startswith ("#[["): return

	add_module_to_output_file (dest_dir, name, dest_file)

	content = content[content.find("\n")+1:content.find("]]")]
	content = process_module_includes (content)

	if category == "JUCE module inclusion" and name != "AllLemonsModules":
		content = process_module_targets (content, file_path)

	output_file = os.path.join (dest_dir, name + ".md")

	with open (output_file, "w") as f:
		f.write ("# {n}		{{#{n}}}".format(n=name))
		f.write ("\r\n")
		f.write ("[Back to CMake modules reference](@ref {n})".format(n="CMake_modules"))
		f.write ("\r\n\r\n")
		f.write (content)
