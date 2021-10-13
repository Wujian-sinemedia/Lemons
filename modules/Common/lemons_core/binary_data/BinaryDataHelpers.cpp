#if LEMONS_HAS_BINARY_DATA
#    include "BinaryData.h"
#endif


namespace lemons
{
#if LEMONS_HAS_BINARY_DATA

RawData::RawData (const String& fileToFind)
    : RawData (fileToFind.toRawUTF8())
{
}

RawData::RawData (const char* fileToFind)
{
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
}


bool RawData::isValid() const
{
    return data != nullptr && size > 0;
}


#else /* if LEMONS_HAS_BINARY_DATA */

RawData::RawData (const String&)
{
}

RawData::RawData (const char*)
{
}

bool RawData::isValid() const { return false; }

#endif

}  // namespace lemons
