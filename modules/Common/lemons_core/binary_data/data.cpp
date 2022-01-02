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

#include <lemons_midi/lemons_midi.h>

namespace lemons::binary
{

Data::Data (const String& fileToFind)
{
#if LEMONS_HAS_BINARY_DATA
	data = [&]() -> const char*
	{
		using namespace BinaryData;

		const auto utf8 = fileToFind.toRawUTF8();

		for (int index = 0; index < namedResourceListSize; ++index)
		{
			const auto binaryName = namedResourceList[index];

			if (getNamedResourceOriginalFilename (binaryName) == utf8)
				return getNamedResource (binaryName, size);
		}

		return nullptr;
	}();
#else
	juce::ignoreUnused (fileToFind);
#endif
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

	return { juce::CharPointer_UTF8 (data) };
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
    
    for (int index = 0; index < namedResourceListSize; ++index)
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


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if 0

namespace lemons::tests
{

BinaryDataTests::BinaryDataTests()
: Test ("Binary data tests", "BinaryData")
{

}

#  if ! LEMONS_BINARIES_UNIT_TESTS

void BinaryDataTests::runTest()
{
    beginTest ("binary::Data objects are invalid");

    binary::Data data { "MyFile.txt" };

    expect (! data.isValid());
}

#  else /* LEMONS_BINARIES_UNIT_TESTS is true... */

void BinaryDataTests::runTest()
{

}


#  endif

}

#endif
