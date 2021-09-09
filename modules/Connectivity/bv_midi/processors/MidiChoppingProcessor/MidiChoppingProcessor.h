#pragma once

namespace bav::dsp
{
template < typename SampleType >
class MidiChoppingProcessor
{
public:
    virtual ~MidiChoppingProcessor() = default;

    void prepare (int maxBlocksize);

    void process (AudioBuffer< SampleType >& audio, MidiBuffer& midi);

    void processBypassed (int numSamples, MidiBuffer& midi);

private:
    void processInternal (AudioBuffer< SampleType >& audio, MidiBuffer& midi,
                          int startSample, int numSamples);

    virtual void handleMidiMessage (const MidiMessage& m)                         = 0;
    virtual void renderChunk (AudioBuffer< SampleType >& audio, MidiBuffer& midi) = 0;

    MidiBuffer                midiStorage;
    AudioBuffer< SampleType > dummyBuffer;
};

}  // namespace bav::dsp
