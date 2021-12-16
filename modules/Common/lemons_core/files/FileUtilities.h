/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */

#pragma once

namespace lemons::files
{

using juce::ValueTree;
using juce::File;

/** An enum used to describe various kinds of binary encodings of serialized data. */
enum class FileType
{
	XML, /** Represents an XML file. */
	JSON, /** Represents a JSON file. */
	Opaque /** Represents any non-standard/proprietary method of encoding and decoding information to/from binary. May not be human-readable. */
};

/** Attempts to load a ValueTree from a file of the specified format. */
template <FileType Type = FileType::JSON>
[[nodiscard]] ValueTree loadValueTree (const File& file);

/** Saves a ValueTree to a file in the specified format. */
template <FileType Type = FileType::JSON>
bool saveValueTree (const File& file, const ValueTree& tree);

/** Returns true if the file has a file extension matching any standard MIDI file extensions. */
[[nodiscard]] bool isMidiFile (const File& file);

/** Returns a file on the desktop with the specified name. */
[[nodiscard]] File getFileOnDesktop (const String& fileName);

}  // namespace lemons::files
