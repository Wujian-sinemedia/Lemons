
#include "bv_core.h"

#include "math/mathHelpers.cpp"
#include "math/vecops/vecops.cpp"
#include "math/intops/intOps.cpp"

#include "misc/misc.cpp"

#include "async/AsyncUtils.cpp"

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

#include "files/FileUtilities.cpp"
#include "files/BinaryDataHelpers/BinaryDataHelpers.cpp"

#include "building_blocks/ProcessorBase/ProcessorBase.cpp"
#include "building_blocks/EditorBase/EditorBase.cpp"

#include "parameters/Parameters.cpp"
#include "parameters/ParameterList/ParameterList.cpp"
