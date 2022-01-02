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

/** @defgroup lemons_files Files
    @ingroup lemons_core
    Utilities for working with files.
 */

/** @ingroup lemons_files
    An enum used to describe various kinds of binary encodings of serialized data. */
enum class FileType
{
	/** Represents an XML file. */
	XML,

	/** Represents a JSON file. */
	JSON,

	/** Represents any non-standard/proprietary method of encoding and decoding information to/from binary. May not be human-readable. */
	Opaque
};


/** @defgroup lemons_value_trees ValueTrees
    @ingroup lemons_files
    ValueTree utility functions.
 */


/** @ingroup lemons_value_trees
    Attempts to load a ValueTree from a file of the specified format. The default format is JSON.
    @see saveValueTree()
 */
template <FileType Type = FileType::JSON>
[[nodiscard]] ValueTree loadValueTree (const File& file);

/** @ingroup lemons_value_trees
    Attempts to load a ValueTree from the contents of a file of the specified format. The default format is JSON.
    @see saveValueTree()
 */
template <FileType Type = FileType::JSON>
[[nodiscard]] ValueTree loadValueTree (const String& fileContents);

/** @ingroup lemons_value_trees
    Saves a ValueTree to a file in the specified format.
    @see loadValueTree()
 */
template <FileType Type = FileType::JSON>
bool saveValueTree (const File& file, const ValueTree& tree);




/** @name MemoryBlocks
    @ingroup lemons_files
 */
///@{

/** Loads a File into a MemoryBlock.
 @see saveBlockToFile()
 */
[[nodiscard]] MemoryBlock loadFileAsBlock (const File& file);

/** Writes the block of data into the File.
 @see loadFileAsBlock()
 */
bool saveBlockToFile (const MemoryBlock& block, const File& file);

///@}


/** @ingroup lemons_files
    Returns true if the file has a file extension matching any standard MIDI file extensions. */
[[nodiscard]] bool isMidiFile (const File& file);

/** @ingroup lemons_files
    Returns a file on the desktop with the specified name. */
[[nodiscard]] File getFileOnDesktop (const String& fileName);

}  // namespace lemons::files
