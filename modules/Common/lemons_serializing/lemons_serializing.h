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


#include "SerializableData/MapInterface.h"
#include "SerializableData/ContainerInterface.h"
#include "SerializableData/VarConversion.h"

#include "SerializableData/TreeReflector/TreeReflector.h"

#include "SerializableData/SerializableData.h"

#include "Serializing/Serializing.h"

#include "version/Version.h"

#include "undo/UndoManager.h"

#include "toggler/Toggler.h"

#include "DataSynchronizer/DataSynchronizer.h"
