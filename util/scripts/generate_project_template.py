#!/usr/bin/env python

import os
import shutil
import argparse


###############################################################################

# Sets up a local project repo based on a template

def set_up_repo (template_name, output_dir, new_repo_name):

	if not (template_name == "DefaultJuceApp" or template_name == "DefaultJucePlugin"):
		raise Exception ("Unknown project template requested: " + template_name)
		return

	if os.path.isdir (output_dir):
		shutil.rmtree (output_dir)

	os.mkdir (output_dir)
	os.chdir (output_dir)

	os.system ("gh repo create " + new_repo_name + " --template=\"{url}\"".format (url="https://github.com/benthevining/" + template_name + ".git"))

	# Process -- change class names, etc...




###############################################################################

# Main script

if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument ("template_name", help="the name of the template repository. Either \'DefaultJuceApp\' or \'DefaultJucePlugin\'.")
	parser.add_argument ("output_dir", help="the directory in which to create the new project repository")
	parser.add_argument ("new_repo_name", help="the name of the new project repo that will be generated")
	
	args = parser.parse_args()

	set_up_repo (args.template_name, args.output_dir, args.new_repo_name)

