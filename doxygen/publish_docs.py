#!/usr/bin/env python

# This script is based on: https://gist.github.com/vidavidorra/548ffbcdae99d752da02
# This is intended for CI machines automatically building & pushing new versions of the docs onto the 'docs' branch, for publishing on the GitHub Pages site.

import os
import shutil
import argparse


if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument ("github_repo_token", help="secure token for github login")
	
	args = parser.parse_args()

	script_dir = os.path.abspath (os.path.dirname (__file__))

	repo_url = "github.com/benthevining/Lemons.git"

	os.chdir (script_dir)

	os.system ("git clone -b docs https://" + repo_url)

	working_dir = os.path.join (script_dir, Lemons)

	# remove everything currently in the docs branch 
	for dirpath, dirnames, filenames in os.walk (working_dir):
		# subdirs
		for subdir in dirnames:
			path = os.path.join (dirpath, dirnames)
			if os.path.isdir (path):
				if not (subdir == ".git"):
					shutil.rmtree (path)
		# top-level files
		for file in filenames:
			path = os.path.join (dirpath, file)
			if (os.exists (path)):
				os.remove (path)

	# Build new copy of docs
	os.chdir (script_dir)
	os.system ("make clean")
	os.system ("make")

	# Copy new docs into git tree
	shutil.copytree (os.path.join (script_dir, "doc"), working_dir)

	# Need to create an empty .nojekyll file for GH pages...
	with open (os.path.join (working_dir, ".nojekyll"), "w") as f:
		f.write ("")

	# Publish new documentation
	os.system ("git config --global push.default simple")
	os.system ("git config user.name \"Github Actions\"")
	os.system ("git config user.email \"actions@github.com\"")
	os.system ("git commit -a -m \"Updating documentation\"")
	os.system ("git push --force \"https://{t}@{url}\"".format (t=os.environ['GH_REPO_TOKEN'], url=repo_url))
