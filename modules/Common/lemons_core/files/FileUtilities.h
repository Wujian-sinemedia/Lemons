
#pragma once

namespace lemons
{
File getFileOnDesktop (const String& fileName);


//==============================================================================


String addFileExtensionIfMissing (const String& string,
                                  const String& extension);

String removeFileExtensionIfThere (const String& string,
                                   const String& extension);


//==============================================================================


bool renameFile (const File& f, const String& newName);

void deleteFile (const File& f);


//==============================================================================
/*
 Opens a file in its default application.
 */
bool openFile (File file);


}  // namespace lemons
