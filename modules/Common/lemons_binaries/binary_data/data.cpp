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
		using namespace LEMONS_BINARY_DATA_NAMESPACE;

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

constexpr bool Data::hasBinaryData() noexcept
{
#if LEMONS_HAS_BINARY_DATA
    return true;
#else
    return false;
#endif
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


MemoryBlock Data::getBlob (const String& filename)
{
	const Data d { filename };

	jassert (d.isValid());

	return d.getAsMemoryBlock();
}

template<files::FileType Type>
ValueTree Data::getValueTree (const String& filename)
{
    return files::loadValueTree<Type> (getString (filename));
}

template ValueTree Data::getValueTree<files::FileType::XML> (const String&);
template ValueTree Data::getValueTree<files::FileType::JSON> (const String&);
template ValueTree Data::getValueTree<files::FileType::Opaque> (const String&);

Image Data::getImage (const String& imageFileName)
{
	const Data d { imageFileName };

	jassert (d.isValid());

	return juce::ImageCache::getFromMemory (d.data, d.size);
}

AudioFile Data::getAudio (const String& audioFileName)
{
	AudioFile audio { std::make_unique<juce::MemoryInputStream> (getBlob (audioFileName), false) };

	jassert (audio.isValid());

	return audio;
}

MidiBuffer Data::getMidi (const String& midiFileName)
{
	return serializing::midiFromBinary (getBlob (midiFileName));
}

String Data::getString (const String& textFileName)
{
	const Data d { textFileName };

	jassert (d.isValid());

	return d.getAsString();
}

juce::StringArray Data::getStrings (const String& textFileName)
{
	return juce::StringArray::fromTokens (getString (textFileName), "\n\r\n", "");
}

std::unique_ptr<juce::CustomTypeface> Data::getFont (const String& fontFileName)
{
    const auto block = getBlob (fontFileName);

    juce::MemoryInputStream is { block, false };
    
    return std::make_unique<juce::CustomTypeface> (is);
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
