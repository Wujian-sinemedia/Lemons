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


#if LEMONS_HAS_BINARY_DATA
#  include "BinaryData.h"
#endif

namespace lemons::binary
{

Data::Data (const char* fileToFind)
{
#if LEMONS_HAS_BINARY_DATA
	data = [this, fileToFind]() -> const char*
	{
		using namespace BinaryData;

		for (int index = 0; index < namedResourceListSize; ++index)
		{
			const auto binaryName = namedResourceList[index];

			if (std::strcmp (getNamedResourceOriginalFilename (binaryName), fileToFind) == 0)
				return getNamedResource (binaryName, size);
		}

		return nullptr;
	}();
#else
	juce::ignoreUnused (fileToFind);
#endif
}

Data::Data (const String& fileToFind)
	: Data (fileToFind.toRawUTF8())
{
}

const char* const Data::getData() const noexcept
{
	return data;
}

int Data::getSize() const noexcept
{
	return size;
}

bool Data::isValid() const noexcept
{
	return data != nullptr && size > 0;
}

String Data::getAsString() const
{
	if (! isValid())
		return {};

	return String::createStringFromData (data, size);
}

MemoryBlock Data::getAsMemoryBlock() const
{
	if (! isValid())
		return {};

	return { data, static_cast<size_t> (size) };
}


/*------------------------------------------------------------------------------------*/

constexpr bool hasBinaryData() noexcept
{
#if LEMONS_HAS_BINARY_DATA
	return true;
#else
	return false;
#endif
}

juce::StringArray getFilenames()
{
	juce::StringArray filenames;

#if LEMONS_HAS_BINARY_DATA

	using namespace BinaryData;

	for (auto index = 0; index < namedResourceListSize; ++index)
	{
		filenames.add (originalFilenames[index]);
	}
#endif

	return filenames;
}

MemoryBlock getBlob (const String& filename)
{
	const Data d { filename };

	jassert (d.isValid());

	return d.getAsMemoryBlock();
}

template <files::FileType Type>
ValueTree getValueTree (const String& filename)
{
	return files::loadValueTree<Type> (getString (filename));
}

template ValueTree getValueTree<files::FileType::XML> (const String&);
template ValueTree getValueTree<files::FileType::JSON> (const String&);
template ValueTree getValueTree<files::FileType::Opaque> (const String&);


String getString (const String& textFileName)
{
	const Data d { textFileName };

	jassert (d.isValid());

	return d.getAsString();
}

juce::StringArray getStrings (const String& textFileName)
{
	return juce::StringArray::fromTokens (getString (textFileName), "\n\r\n", "");
}

}  // namespace lemons::binary
