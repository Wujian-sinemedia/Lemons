#!/usr/bin/env python

import os
import shutil


###############################################################################

# Clones a copy of juce and returns its path
def clone_juce (script_dir):
	print ("Cloning JUCE...")
	os.chdir (script_dir)
	os.system ("git clone https://github.com/juce-framework/JUCE.git")
	return os.path.join (script_dir, "JUCE")


# Attempts to find JUCE within the CPM source cache; otherwise clones a new copy of JUCE
def find_juce_path (script_dir):
	# This is where the cache is in my standard workspace setup...
	cache_dir = os.path.join (os.path.dirname (os.path.dirname (script_dir)), "Cache")

	juce_base_dir = os.path.join (cache_dir, "juce")

	if not os.path.isdir (juce_base_dir):
		return clone_juce (script_dir)

	for item in os.listdir (juce_base_dir):
		subdir = os.path.join (juce_base_dir, item)
		if os.path.isdir (subdir):
			# Test to see if juce modules directory exists
			if os.path.isdir (os.path.join (subdir, "modules")):
				return subdir

	return clone_juce (script_dir)

###############################################################################

if __name__ == "__main__":

	script_dir = os.path.abspath (os.path.dirname (__file__))

	juce_path = find_juce_path (script_dir)

	juce_doxygen_dir = os.path.join (juce_path, "docs")
	juce_doxygen_dir = os.path.join (juce_doxygen_dir, "doxygen")

	os.chdir (juce_doxygen_dir)
	os.system ("make clean && make")

	juce_doxygen_output = os.path.join (juce_doxygen_dir, "doc")

	output_dir = os.path.join (script_dir, "build")

	shutil.copy2 (os.path.join (juce_doxygen_output, "juce.tag"), 
		          os.path.join (output_dir, "juce.tag"))

