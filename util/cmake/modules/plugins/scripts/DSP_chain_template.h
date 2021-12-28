#pragma once

#include <lemons_dsp/lemons_dsp.h>

namespace @NAMESPACE@
{

template<typename SampleType>
using @CLASS_NAME@ = lemons::dsp::BuiltEngineChain<SampleType, @ENGINE_TYPES@>;

}
