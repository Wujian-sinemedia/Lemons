#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                lemons_serializing
 vendor:            Ben Vining
 version:           0.0.1
 name:              lemons_serializing
 description:       Easily serialize any C++ object to and from ValueTree, JSON, or XML
 dependencies:      lemons_core

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/

#include <juce_core/juce_core.h>

/*
 For any type you want to be able to serialize with treeReflector.add(YourType), simply implement these two functions within the lemons namespace.
 */
namespace lemons
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
