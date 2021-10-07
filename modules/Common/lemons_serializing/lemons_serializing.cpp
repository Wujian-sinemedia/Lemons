#include "lemons_serializing.h"


#include "SerializableData/TreeReflector/TreeReflector.cpp"
#include "SerializableData/SerializableData.cpp"

#include "Serializing/Specializations/containers/Juce_containers.cpp"
#include "Serializing/Specializations/maps/Juce_maps.cpp"
#include "Serializing/Specializations/VarSpecializations.cpp"
#include "Serializing/ValueTreeToJson/valuetree_json_converter.cpp"
#include "Serializing/Serializing.cpp"

#include "undo/UndoManager.cpp"

#include "DataSynchronizer/DataSynchronizer.cpp"

#include "version/Version.cpp"
