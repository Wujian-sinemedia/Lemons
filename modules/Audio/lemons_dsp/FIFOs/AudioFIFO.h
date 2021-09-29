#pragma once

namespace lemons::dsp
{
/* FIFO for a single channel of audio samples */
template < typename SampleType >
class AudioFIFO
{
public:
    void pushSamples (const SampleType* samples, int numSamples);

    void popSamples (SampleType* output, int numSamples);

    int numStoredSamples() const;

    void setMaximumSize (int maximumCapacitySamples);

private:
    CircularBuffer< SampleType > buffer;

    int writeHead {0};
    int numStored {0};
};


/* FIFO for multiple channels of audio */
template < typename SampleType >
class MultiAudioFIFO
{
public:
    MultiAudioFIFO (int numChannels = 2, int maxCapacity = 1024);

    void setNumChannels (int numChannels);

    void setMaximumSize (int maximumCapacitySamples);

    int numStoredSamples() const;

    void pushSamples (const AudioBuffer< SampleType >& input);

    void popSamples (AudioBuffer< SampleType >& output);

private:
    OwnedArray< AudioFIFO< SampleType > > fifos;

    int capacity {512};
};

}  // namespace bav::dsp
