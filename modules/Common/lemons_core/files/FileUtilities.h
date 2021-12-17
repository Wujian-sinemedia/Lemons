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

#include <juce_data_structures/juce_data_structures.h>

namespace lemons::files
{

using juce::File;
using juce::MemoryBlock;
using juce::ValueTree;

/** An enum used to describe various kinds of binary encodings of serialized data. */
enum class FileType
{
	XML,   /** Represents an XML file. */
	JSON,  /** Represents a JSON file. */
	Opaque /** Represents any non-standard/proprietary method of encoding and decoding information to/from binary. May not be human-readable. */
};

/** Attempts to load a ValueTree from a file of the specified format.
    @see saveValueTree()
 */
template <FileType Type = FileType::JSON>
[[nodiscard]] ValueTree loadValueTree (const File& file);

/** Attempts to load a ValueTree from the contents of a file of the specified format.
    @see saveValueTree()
 */
template <FileType Type = FileType::JSON>
[[nodiscard]] ValueTree loadValueTree (const String& fileContents);

/** Saves a ValueTree to a file in the specified format.
    @see loadValueTree()
 */
template <FileType Type = FileType::JSON>
bool saveValueTree (const File& file, const ValueTree& tree);

/** Returns true if the file has a file extension matching any standard MIDI file extensions. */
[[nodiscard]] bool isMidiFile (const File& file);

/** Returns a file on the desktop with the specified name. */
[[nodiscard]] File getFileOnDesktop (const String& fileName);

/** Loads a File into a MemoryBlock.
    @see saveBlockToFile()
 */
[[nodiscard]] MemoryBlock loadFileAsBlock (const File& file);

/** Writes the block of data into the File.
    @see loadFileAsBlock()
 */
bool saveBlockToFile (const MemoryBlock& block, const File& file);

}  // namespace lemons::files
