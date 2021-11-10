#!/usr/bin/env python

from os import path

#

# Returns the key (to be translated) from a line of a translation file

def get_token_for_line (line):

	if not line: return ""

	idx = line.find ("\" =")

	if idx < 0: return ""

	token = line[:idx]

	if token.startswith ("\""): token = token[1:]

	return token.strip()

#

# Returns a list of tokens present in a JUCE translation file

def get_tokens_in_file (file_path):

	tokens = []

	if not path.exists (file_path): return tokens

	with open (file_path, "r") as f:
		for line in f:
			token = get_token_for_line (line)
			if token:
				tokens.append (token)

	return tokens
