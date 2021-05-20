
namespace bav::dsp
{
/* A FIFO that manages both audio samples and MIDI events */

template < typename SampleType >
class AudioAndMidiFIFO
{
public:
    AudioAndMidiFIFO() { }

    void initialize (const int numChannels, const int size)
    {
        audio.initialize (numChannels, size);
        midi.setSize (size);
    }

    void clear()
    {
        audio.clear();
        midi.clear();
    }

    void changeSize (const int newNumChannels, int newSize)
    {
        audio.changeSize (newNumChannels, newSize);
        midi.setSize (newSize);
    }


    void push (const juce::AudioBuffer< SampleType >& inputAudio,
               const juce::MidiBuffer&                inputMidi,
               const int                              numSamples)
    {
        midi.pushEvents (inputMidi, numSamples);

        for (int i = 0;
             i < std::min (inputAudio.getNumChannels(), audio.getNumChannels());
             ++i)
            audio.pushSamples (inputAudio.getReadPointer (i), numSamples, i);
    }


    void pop (juce::AudioBuffer< SampleType >& outputAudio,
              juce::MidiBuffer&                outputMidi,
              const int                        numSamples)
    {
        midi.popEvents (outputMidi, numSamples);

        for (int i = 0;
             i < std::min (outputAudio.getNumChannels(), audio.getNumChannels());
             ++i)
            audio.popSamples (outputAudio.getWritePointer (i), numSamples, i);
    }


    int numStoredMidiEvents() const { return midi.numStoredEvents(); }

    int numStoredSamples() const { return audio.numStoredSamples(); }


private:
    AudioFIFO< SampleType > audio;
    midi::MidiFIFO          midi;
};


template class AudioAndMidiFIFO< float >;
template class AudioAndMidiFIFO< double >;


}  // namespace bav::dsp
