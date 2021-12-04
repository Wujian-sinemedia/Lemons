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
		using namespace BinaryData;

		for (int index = 0; index < namedResourceListSize; ++index)
		{
			const auto binaryName = namedResourceList[index];

			if (getNamedResourceOriginalFilename (binaryName) == fileToFind.toRawUTF8())
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


template <typename SampleType>
AudioBuffer<SampleType> Data::getAudio (const String& audioFileName)
{
	return audioFromBinary<SampleType> (getBlob (audioFileName));
}

template AudioBuffer<float>  Data::getAudio (const String&);
template AudioBuffer<double> Data::getAudio (const String&);


MidiBuffer Data::getMidi (const String& midiFileName)
{
	return midiFromBinary (getBlob (midiFileName));
}

juce::StringArray Data::getStrings (const String& textFileName)
{
	Data d { textFileName };

	jassert (d.isValid());

	return juce::StringArray::fromTokens (d.getAsString(), "\n\r\n", "");
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

#if ! LEMONS_BINARIES_UNIT_TESTS

void BinaryDataTests::runTest()
{
    beginTest ("binary::Data objects are invalid");

    binary::Data data { "MyFile.txt" };

    expect (! data.isValid());
}

#else /* LEMONS_BINARIES_UNIT_TESTS is true... */

void BinaryDataTests::runTest()
{

}


#endif

}

#endif
