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

	output_file = os.path.join (dest_dir, name + ".md")

	with open (output_file, "w") as f:
		f.write ("# {n}		{{#{n}}}".format(n=name))
		f.write ("\r\n")
		f.write ("#### Category: {c}".format(c=category.capitalize()))
		f.write ("\r\n")
		f.write (content)
