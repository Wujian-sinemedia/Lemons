#!/usr/bin/env python

import os
import shutil

from argparse import ArgumentParser

#

if __name__ == "__main__":

	GH_REPO_REF = "github.com/benthevining/Lemons.git"

	script_dir = os.path.abspath (os.path.dirname (__file__))
	lemons_root = os.path.abspath (os.path.dirname (script_dir))

	parser = ArgumentParser()
	parser.add_argument ("GH_REPO_TOKEN", help="token for committing to the Lemons docs branch")
	
	args = parser.parse_args()

	temp_dir = os.path.join (script_dir, "docs")

	if os.path.isdir (temp_dir): 
		shutil.rmtree (temp_dir)
	
	os.mkdir (temp_dir)

	os.chdir (temp_dir)

	os.system ("git clone -b docs --recurse-submodules \"https://git@{r}\"".format(r=GH_REPO_REF))

	docs_git_tree = os.path.join (temp_dir, "Lemons")

	# remove everything currently in the docs branch
	os.chdir (docs_git_tree)
	os.system ("rm -rf *")

	# generate the doxygen documentation
	os.chdir (lemons_root)
	os.system ("cmake --preset docs")
	os.system ("cmake --build --preset docs")

	# copy generated docs to cloned copy of docs git tree
	shutil.copytree (os.path.join (lemons_root, "docs"), 
		             docs_git_tree,
		             dirs_exist_ok=True)

	# need to create an empty .nojekyll file
	with open (os.path.join (docs_git_tree, ".nojekyll"), "w") as f:
		f.write ("")

	os.chdir (docs_git_tree)

	# configure git
	os.system ("git config push.default simple")
	os.system ("git config user.name \"Github Actions\"")
	os.system ("git config user.email \"actions@github.com\"")

	os.system ("git add --all")
	os.system ("git commit -am \"Updating docs\"")
	os.system ("git push --force \"https://{t}@{r}\"".format(t=args.GH_REPO_TOKEN, r=GH_REPO_REF))
