
#pragma once

namespace lemons::dsp
{
template < typename SampleType >
class Engine
{
public:
    Engine()          = default;
    virtual ~Engine() = default;

    virtual int reportLatency() const { return 0; }

    void process (AudioBuffer< SampleType >& inplaceInAndOut,
                  MidiBuffer&                midiMessages,
                  bool                       isBypassed = false);

    void process (const AudioBuffer< SampleType >& input,
                  AudioBuffer< SampleType >&       output,
                  MidiBuffer&                      midiMessages,
                  bool                             isBypassed = false);

    // call these two functions to use the engine without MIDI
    // (the engine's internal renderBlock() will recieve an empty midiBuffer)
    void process (AudioBuffer< SampleType >& inplaceInAndOut,
                  bool                       isBypassed = false);

    void process (const AudioBuffer< SampleType >& input,
                  AudioBuffer< SampleType >&       output,
                  bool                             isBypassed = false);

    bool isInitialized() const { return hasBeenInitialized; }

    void prepare (double samplerate, int blocksize, int numChannels = 2);

    void releaseResources();

    double getSamplerate() const { return sampleRate; }

private:
    void processInternal (const AudioBuffer< SampleType >& input, AudioBuffer< SampleType >& output, MidiBuffer& midiMessages, bool isBypassed);

    virtual void renderBlock (const AudioBuffer< SampleType >& input, AudioBuffer< SampleType >& output, MidiBuffer& midiMessages, bool isBypassed) = 0;
    virtual void prepared (int blocksize, double samplerate);
    virtual void released() { }

    MidiBuffer dummyMidiBuffer;
    bool       wasBypassedLastCallback {true};
    bool       hasBeenInitialized {false};
    double     sampleRate {0.};

    AudioBuffer< SampleType > outputStorage;
};

}  // namespace bav::dsp
