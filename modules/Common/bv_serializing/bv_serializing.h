#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                bv_serializing
 vendor:            Ben Vining
 version:           0.0.1
 name:              bv_serializing
 description:       Easily serialize any C++ object to and from a ValueTree or JSON
 dependencies:      bv_core

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/

#include <juce_core/juce_core.h>

/*
 For any type you want to be able to serialize with treeReflector.add(YourType), simply implement these two functions within the bav namespace.
 */
namespace bav
{
template < typename Type >
juce::var toVar (Type& object)
{
    return {object};
}

template < typename Type >
Type fromVar (juce::var var)
{
    return {var};
}

}  // namespace bav


#include "SerializableData/SerializableData.h"

#include "Serializing/Serializing.h"

#include "version/Version.h"

#include "undo/UndoManager.h"

#include "DataSynchronizer/DataSynchronizer.h"
