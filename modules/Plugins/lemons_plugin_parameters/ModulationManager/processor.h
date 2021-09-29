#pragma once

namespace lemons::plugin
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
    void processInternal (AudioBuffer< SampleType >& audio, MidiBuffer& midi);

    virtual void renderChunk (AudioBuffer< SampleType >& audio, MidiBuffer& midi) = 0;

    ModulationManager& manager;

    MidiBuffer midiStorage;
};

}  // namespace bav::plugin
