
#include "bv_core.h"

#if BV_USE_VDSP
#    include "math/vecops/implementations/vecops_vdsp.cpp"
#elif BV_USE_IPP
#    include "math/vecops/implementations/vecops_ipp.cpp"
#elif BV_USE_MIPP
#    include "math/vecops/implementations/vecops_mipp.cpp"
#elif BV_USE_NE10
#    include "math/vecops/implementations/vecops_ne10.cpp"
#else
#    include "math/vecops/implementations/vecops_fallback.cpp"
#endif

#include "math/mathHelpers.cpp"
#include "math/vecops/vecops.cpp"
#include "math/intops/intOps.cpp"

#include "misc/misc.cpp"

#include "serializing/SerializableData/SerializableData.cpp"
#include "serializing/SerializableData/Specializations.cpp"
#include "serializing/ValueTreeToJson/valuetree_json_converter.cpp"
#include "serializing/Serializing/Serializing.cpp"

#include "files/FileUtilities.cpp"
#include "binary_data/BinaryDataHelpers.cpp"

#include "localization/localization.cpp"

#include "undo/UndoManager.cpp"

#include "presets/PresetManager.cpp"
