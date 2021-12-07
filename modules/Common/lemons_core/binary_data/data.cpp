#if LEMONS_HAS_BINARY_DATA
#  include "BinaryData.h"
#endif


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


bool Data::isValid() const noexcept
{
#if LEMONS_HAS_BINARY_DATA
	return data != nullptr && size > 0;
#else
	return false;
#endif
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
	Data d { filename };

	jassert (d.isValid());

	return d.getAsMemoryBlock();
}

Image Data::getImage (const String& imageFileName)
{
	Data d { imageFileName };

	jassert (d.isValid());

	return juce::ImageCache::getFromMemory (d.data, d.size);
}

AudioFile Data::getAudio (const String& audioFileName)
{
	auto stream = std::make_unique<juce::MemoryInputStream> (getBlob (audioFileName), false);

	AudioFile audio { std::move (stream) };

	jassert (audio.isValid());

	return audio;
}

MidiBuffer Data::getMidi (const String& midiFileName)
{
	return serializing::midiFromBinary (getBlob (midiFileName));
}

String Data::getString (const String& textFileName)
{
	Data d { textFileName };

	jassert (d.isValid());

	return d.getAsString();
}

juce::StringArray Data::getStrings (const String& textFileName)
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
