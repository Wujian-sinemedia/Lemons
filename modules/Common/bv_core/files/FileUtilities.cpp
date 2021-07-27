
namespace bav
{
File getFileOnDesktop (const String& fileName)
{
    return File::getSpecialLocation (File::userDesktopDirectory).getChildFile (fileName);
}


//==============================================================================


String addFileExtensionIfMissing (const String& string,
                                  const String& extension)
{
    return File::createLegalFileName (
        string.endsWith (extension) ? string.trim() : (string + extension).trim());
}


String removeFileExtensionIfThere (const String& string,
                                   const String& extension)
{
    return (string.endsWith (extension))
             ? string.dropLastCharacters (extension.length()).trim()
             : string.trim();
}


//==============================================================================

bool renameFile (const File& f, const String& newName)
{
    jassert (! newName.isEmpty());

    if (! f.existsAsFile())
        return false;

    return f.moveFileTo (f.getSiblingFile (newName));
}

void deleteFile (const File& f)
{
    if (f.existsAsFile())
        if (! f.moveToTrash())
            f.deleteFile();
}

//==============================================================================

bool openFile (File file)
{
    if (! file.existsAsFile())
        return false;

    juce::Process::openDocument ("file:" + file.getFullPathName(), {});
    return true;
}

}  // namespace bav
