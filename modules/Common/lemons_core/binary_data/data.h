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

#include <lemons_core/lemons_core.h>


namespace lemons::binary
{

using juce::MemoryBlock;
using juce::String;
using juce::ValueTree;

/** @defgroup lemons_binary Binary data
	@ingroup lemons_core
	Utilities for working with embedded binary data.
 */

/** @ingroup lemons_binary
	Represents a wrapper around some data contained in a juce BinaryData target.
	Simply provide the filename and this object will load the data contained by the named resource, if it can be located.
	@attention Be sure to check isValid() before attempting to use the data! \n
	For example:
	@code
	Data data {"MyAudioFile.wav"};

	if (data.isValid())
		doSomethingWithData (data.data, data.size);
	@endcode
	If your project doesn't define the LEMONS_HAS_BINARY_DATA macro to 1, every Data object you create will be invalid. \n

	To easily access binary data converted into higher level types, several free functions are provided in this namespace. These methods all throw assertions if the data cannot be loaded. \n
	For example:
	@code
	auto image = binary::getImage ("myImage.jpg");
	@endcode
 */
struct Data final
{
	/** Creates a default, invalid Data object. */
	explicit Data() = default;

	/** Creates a Data object referencing a named resource. */
	explicit Data (const char* fileToFind);

	/** Creates a Data object referencing a named resource. */
	explicit Data (const String& fileToFind);

	/** Returns true if the requested data has been loaded successfully from the binary data target. */
	[[nodiscard]] bool isValid() const noexcept;

	/** Returns the data as a UTF-8 formatted string. */
	[[nodiscard]] String getAsString() const;

	/** Returns a memory block representing this data. */
	[[nodiscard]] MemoryBlock getAsMemoryBlock() const;

	/** Returns a pointer to the raw data. This may be null if the data could not be loaded. */
	[[nodiscard]] const char* const getData() const noexcept;

	/** Return the size of the data, in bytes. */
	[[nodiscard]] int getSize() const noexcept;

private:
	const char* data { nullptr };

	int size { 0 };
};


/** @ingroup lemons_binary
	Returns true if this module was compiled with the LEMONS_HAS_BINARY_DATA flag set to 1.
	If this returns false, every Data object you create will be invalid.
 */
[[nodiscard]] constexpr bool hasBinaryData() noexcept;


/** @ingroup lemons_binary
	Returns a list of file names that are available in the binary data target, ie, filenames that are valid arguments to the Data constructor.
 */
[[nodiscard]] juce::StringArray getFilenames();


/** @ingroup lemons_binary
	Returns an opaque blob of binary data from a file in the BinaryData target.
	If the data can't be loaded, an assertion will be thrown.
	@see files::loadFileAsBlock(), files::saveBlockToFile()
 */
[[nodiscard]] MemoryBlock getBlob (const String& filename);

/** @ingroup lemons_binary
	Returns a ValueTree from a serialized file in the BinaryData target in the specified format. The default format is JSON.
	If the data can't be loaded, an assertion will be thrown.
	@see files::loadValueTree(), files::saveValueTree()
 */
template <files::FileType Type = files::FileType::JSON>
[[nodiscard]] ValueTree getValueTree (const String& filename);

/** @ingroup lemons_binary
	Returns a text file in the BinaryData target as one string.
	If the data can't be loaded, an assertion will be thrown.
	@see getStrings()
 */
[[nodiscard]] String getString (const String& textFileName);

/** @ingroup lemons_binary
	Returns an array of strings, each containing a line of a text file in the BinaryData target.
	This loads the file as a string and parses it into tokens using line break and carriage return characters.
	If the file can't be loaded, an assertion will be thrown.
	@see getString()
 */
[[nodiscard]] juce::StringArray getStrings (const String& textFileName);


}  // namespace lemons::binary
