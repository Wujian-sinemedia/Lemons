
/*
    BinaryDataHelpers.h
 
    This is the only file in this module not included by the bv_SharedCode header, because not every JUCE project will have a BinaryData target.
    To use the functions in this file, you MUST have JUCE autogenerate a BinaryData target containing the files you want to access.
*/


#pragma once

#include "BinaryData.h"
#include <juce_graphics/juce_graphics.h>


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
    
}  // namespace
