
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class LatencyEngine : public Engine< SampleType >
{
public:
    int  reportLatency() const final { return internalBlocksize; }
    void changeLatency (int newInternalBlocksize);

private:
    void renderBlock (const AudioBuffer< SampleType >& input, AudioBuffer< SampleType >& output, MidiBuffer& midiMessages, bool isBypassed) final;

    void prepared (int blocksize, double samplerate) final;
    void released() final;

    virtual void renderChunk (const AudioBuffer< SampleType >& input, AudioBuffer< SampleType >& output, MidiBuffer& midiMessages, bool isBypassed) = 0;
    virtual void onPrepare (int blocksize, double samplerate);
    virtual void onRelease() { }


    int                            internalBlocksize {0};
    AudioAndMidiFIFO< SampleType > inputFIFO, outputFIFO;
    AudioBuffer< SampleType >      inBuffer, outBuffer;
    MidiBuffer                     chunkMidiBuffer;
};

}  // namespace bav::dsp
