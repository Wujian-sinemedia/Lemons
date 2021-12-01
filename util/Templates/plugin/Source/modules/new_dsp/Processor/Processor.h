#pragma once

#include <new_dsp/Engine/Engine.h>

namespace New
{
class Processor : public plugin::Processor< State, Engine >
{
public:
    Processor();

private:
    bool canAddBus (bool isInput) const final { return isInput; }
    bool isBusesLayoutSupported (const BusesLayout& layouts) const final;

    double getTailLengthSeconds() const final;

    bool acceptsMidi() const final { return true; }
    bool producesMidi() const final { return true; }
    bool supportsMPE() const final { return false; }
    bool isMidiEffect() const final { return false; }

    const String getName() const final { return "NewProject"; }
};

}  // namespace New
