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

namespace lemons::files
{

template <>
ValueTree loadValueTree<FileType::XML> (const String& fileContents)
{
	if (auto xml = std::unique_ptr<juce::XmlElement> (juce::XmlDocument::parse (fileContents)))
		return ValueTree::fromXml (*xml);

	return {};
}

template <>
ValueTree loadValueTree<FileType::XML> (const File& file)
{
	return loadValueTree<FileType::XML> (file.loadFileAsString());
}

template <>
ValueTree loadValueTree<FileType::JSON> (const String& fileContents)
{
	return serializing::valueTreeFromJSON (fileContents);
}

template <>
ValueTree loadValueTree<FileType::JSON> (const File& file)
{
	return loadValueTree<FileType::JSON> (file.loadFileAsString());
}

template <>
ValueTree loadValueTree<FileType::Opaque> (const String& fileContents)
{
	juce::CharPointer_UTF8 stringPointer (fileContents.toUTF8());
	juce::MemoryBlock      stringMemoryBlock (stringPointer.getAddress(), stringPointer.sizeInBytes());

	juce::MemoryInputStream is { stringMemoryBlock, false };

	return ValueTree::readFromStream (is);
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

bool isMidiFile (const File& file)
{
	return file.hasFileExtension ("mid;rmi;rmid;midi");
}


File getFileOnDesktop (const String& fileName)
{
	return File::getSpecialLocation (File::userDesktopDirectory).getChildFile (fileName);
}

MemoryBlock loadFileAsBlock (const File& file)
{
	MemoryBlock block;

	if (auto is = file.createInputStream())
		if (is->openedOk())
			is->readIntoMemoryBlock (block);

	return block;
}

bool saveBlockToFile (const MemoryBlock& block, const File& file)
{
	if (auto os = file.createOutputStream())
		if (os->openedOk())
			return os->write (block.getData(), block.getSize());

	return false;
}

}  // namespace lemons::files
