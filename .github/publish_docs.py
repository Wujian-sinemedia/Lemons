#!/usr/bin/env python

# This script is based on: https://gist.github.com/vidavidorra/548ffbcdae99d752da02
# This is intended for CI machines automatically building & pushing new versions of the docs onto the 'docs' branch, for publishing on the GitHub Pages site.

import os
import shutil

###############################################################################

# Commits to the repo in the specified directory

def commit_to_repo (dir_path):

	os.chdir (dir_path)

	os.system ("git add -f --all")
	os.system ("git commit -a -m \"Updating documentation\"")

###############################################################################

# Main script

if __name__ == "__main__":

	# git config
	os.system ("git config --global push.default simple")
	os.system ("git config user.name \"Github Actions\"")
	os.system ("git config user.email \"actions@github.com\"")


	script_dir = os.path.abspath (os.path.dirname (__file__))

	doxygen_dir = os.path.join (os.path.abspath (os.path.dirname (script_dir)), "doxygen")
	
	repo_url = "github.com/benthevining/Lemons.git"

	os.chdir (doxygen_dir)

	os.system ("git clone -b docs https://" + repo_url)

	working_dir = os.path.join (doxygen_dir, "Lemons")

	os.chdir (working_dir)

	os.system ("git checkout docs")
	os.system ("git fetch && git pull")

	# remove everything currently in the docs branch 
	for dirpath, dirnames, filenames in os.walk (working_dir):
		# subdirs
		for subdir in dirnames:
			path = os.path.join (dirpath, subdir)
			if os.path.isdir (path):
				if not (subdir == ".git"):
					shutil.rmtree (path)
		# top-level files
		for file in filenames:
			path = os.path.join (dirpath, file)
			if (os.path.exists (path)):
				os.remove (path)

	commit_to_repo (working_dir)

	# Build new copy of docs
	os.chdir (doxygen_dir)
	os.system ("make clean")
	os.system ("make")

	# Copy new docs into git tree
	shutil.copytree (os.path.join (doxygen_dir, "doc"), working_dir)

	# Need to create an empty .nojekyll file for GH pages...
	with open (os.path.join (working_dir, ".nojekyll"), "w") as f:
		f.write ("")

	# Publish new documentation

	commit_to_repo (working_dir)
	os.chdir (working_dir)
	os.system ("git push --force origin docs \"https://{t}@{url}\"".format (t=os.environ['GH_REPO_TOKEN'], url=repo_url))

	# Clean up
	os.chdir (doxygen_dir)
	os.system ("make clean")
