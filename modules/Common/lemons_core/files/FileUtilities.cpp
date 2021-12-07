namespace lemons::files
{

template <>
ValueTree loadValueTree<FileType::XML> (const File& file)
{
	if (auto xml = std::unique_ptr<juce::XmlElement> (juce::XmlDocument::parse (file)))
		return ValueTree::fromXml (*xml);

	return {};
}

template <>
ValueTree loadValueTree<FileType::JSON> (const File& file)
{
	return serializing::valueTreeFromJSON (file.loadFileAsString());
}

template <>
ValueTree loadValueTree<FileType::Opaque> (const File& file)
{
	juce::FileInputStream is { file };

	if (is.openedOk())
		return ValueTree::readFromStream (is);

	return {};
}


bool saveValueTreeImpl (const File& file,
                        std::function<void (juce::FileOutputStream&)>
                            writeFunc)
{
	const juce::TemporaryFile temp { file };

	{
		juce::FileOutputStream os { temp.getFile() };

		if (! os.openedOk())
			return false;

		writeFunc (os);
	}

	if (temp.getFile().existsAsFile())
		return temp.overwriteTargetFileWithTemporary();

	return false;
}

template <>
bool saveValueTree<FileType::XML> (const File& file, const ValueTree& tree)
{
	return saveValueTreeImpl (file,
	                          [&] (juce::FileOutputStream& os)
	                          {
		                          if (auto xml = tree.createXml())
			                          xml->writeTo (os);
	                          });
}

template <>
bool saveValueTree<FileType::JSON> (const File& file, const ValueTree& tree)
{
	return saveValueTreeImpl (file,
	                          [&] (juce::FileOutputStream& os)
	                          {
		                          os.writeString (serializing::valueTreeToJSON (tree));
	                          });
}

template <>
bool saveValueTree<FileType::Opaque> (const File& file, const ValueTree& tree)
{
	return saveValueTreeImpl (file,
	                          [&] (juce::FileOutputStream& os)
	                          {
		                          tree.writeToStream (os);
	                          });
}

//==============================================================================

const char* const soundFileWildCard = "*.wav;*.aiff;*.aif;*.ogg;*.mp3;*.flac;*.au;*.voc;*.caf;*.w64;*.rx2;*.rcy;*.rex;*.wfaf";

bool isMidiFile (const File& file)
{
	return file.hasFileExtension ("mid;rmi;rmid;midi");
}


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

}  // namespace lemons::files
