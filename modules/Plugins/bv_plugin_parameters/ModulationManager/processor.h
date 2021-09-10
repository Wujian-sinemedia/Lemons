#pragma once

namespace bav::plugin
{
template < typename SampleType >
class ModulationManagerProcessor
{
public:
    ModulationManagerProcessor (ModulationManager& managerToUse);
    virtual ~ModulationManagerProcessor() = default;

    void prepareToPlay (double samplerate, int blocksize);

    void processBlock (AudioBuffer< SampleType >& audio, MidiBuffer& midi);

private:
    ModulationManager& manager;
};

}  // namespace bav::plugin
