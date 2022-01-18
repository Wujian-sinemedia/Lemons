#pragma once

#include <new_dsp/Engine/Engine.h>

namespace New
{
class Processor : public plugin::Processor<Engine, State>
{
public:
	Processor();
};

}  // namespace New
