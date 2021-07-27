
#pragma once

namespace bav
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


}  // namespace bav
