
#if BV_HAS_BINARY_DATA

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
    
    jassert (data != nullptr);  // File not found
}


#endif /* if BV_HAS_BINARY_DATA */
