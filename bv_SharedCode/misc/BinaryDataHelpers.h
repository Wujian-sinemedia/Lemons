
/*
    BinaryDataHelpers.h
 
    This is the only file in this module not included by the bv_SharedCode header, because not every JUCE project will have a BinaryData target.
    To use the functions in this file, you MUST have JUCE autogenerate a BinaryData target containing the files you want to access.
*/


#include "BinaryData.h"


namespace bav::Assets
{
    
    // returns the available BinaryData filenames as a StringArray
    inline juce::StringArray getFileNames()
    {
        juce::StringArray names;
        
        for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
            names.add (juce::String::fromUTF8 (BinaryData::namedResourceList [i]));
        
        return names;
    }
    
    
    // returns an image loaded into BinaryData as a juce::Image object
    inline juce::Image getImage (const juce::String& name)
    {
        int dataSize = 0;
        const char* data = BinaryData::getNamedResource (name.toRawUTF8(), dataSize);
        
        if (data != nullptr)
            return juce::ImageCache::getFromMemory (data, dataSize);
        
        return {};
    }

    
}  // namespace
