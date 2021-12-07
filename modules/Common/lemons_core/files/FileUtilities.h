#pragma once

namespace lemons::files
{

enum class FileType
{
	XML,
	JSON,
	Opaque
};

template <FileType Type>
[[nodiscard]] ValueTree loadValueTree (const File& file);

template <FileType Type>
bool saveValueTree (const File& file, const ValueTree& tree);


//==============================================================================


[[nodiscard]] bool isMidiFile (const File& file);


[[nodiscard]] File getFileOnDesktop (const String& fileName);


//==============================================================================


String addFileExtensionIfMissing (const String& string,
                                  const String& extension);


//==============================================================================


bool renameFile (const File& f, const String& newName);

void deleteFile (const File& f);


//==============================================================================
/*
 Opens a file in its default application.
 */
bool openFile (File file);


}  // namespace lemons::files
