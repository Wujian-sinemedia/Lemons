#if LEMONS_HAS_BINARY_DATA
#  include "BinaryData.h"
#endif


namespace lemons
{

RawData::RawData (const String& fileToFind)
    : RawData (fileToFind.toRawUTF8())
{
}

RawData::RawData (const char* fileToFind)
{
#if LEMONS_HAS_BINARY_DATA
	data = [&]() -> const char*
	{
		using namespace BinaryData;

		for (int index = 0; index < namedResourceListSize; ++index)
		{
			const auto binaryName = namedResourceList[index];

			if (getNamedResourceOriginalFilename (binaryName) == fileToFind)
				return getNamedResource (binaryName, size);
		}

		return nullptr;
	}();
#else
    juce::ignoreUnused (fileToFind);
#endif
}


bool RawData::isValid() const noexcept
{
#if LEMONS_HAS_BINARY_DATA
	return data != nullptr && size > 0;
#else
    return false;
#endif
}

}  // namespace lemons
