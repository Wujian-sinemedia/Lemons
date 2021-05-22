
#if BV_HAS_BINARY_DATA

namespace bav
{
RawData::RawData (const char* fileToFind)
{
    using namespace BinaryData;

    for (int index = 0; index < namedResourceListSize; ++index)
    {
        auto binaryName = namedResourceList[index];
        auto fileName   = getNamedResourceOriginalFilename (binaryName);

        if (fileName == fileToFind)
        {
            data = BinaryData::getNamedResource (binaryName, size);
            break;
        }
    }
}


bool RawData::isValid() const
{
    return data != nullptr && size > 0;
}


}  // namespace bav

#endif /* if BV_HAS_BINARY_DATA */
