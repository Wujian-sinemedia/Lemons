
namespace lemons
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

	return file.startAsProcess();
}

}  // namespace lemons
