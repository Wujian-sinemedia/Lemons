
namespace lemons::serializing
{
template < typename SampleType >
void bufferChannelLoop (AudioBuffer< SampleType >& buffer,
                        std::function< void (int) >
                            forEachChannel)
{
    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
        forEachChannel (chan);
}
template void bufferChannelLoop (AudioBuffer< float >&, std::function< void (int) >);
template void bufferChannelLoop (AudioBuffer< double >&, std::function< void (int) >);


juce::MemoryBlock toMemory (const AudioBuffer< float >& buffer)
{
    juce::MemoryBlock        block;
    juce::MemoryOutputStream stream {block, false};
    juce::FlacAudioFormat    format;

    if (auto* writer = format.createWriterFor (&stream, 48000, (unsigned) buffer.getNumChannels(), 24, {}, 0))
        writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());

    return block;
}

juce::MemoryBlock toMemory (const AudioBuffer< double >& buffer)
{
    const auto numSamples = buffer.getNumSamples();

    AudioBuffer< float > temp {numSamples, buffer.getNumChannels()};

    bufferChannelLoop (temp,
                       [&] (int chan)
                       { vecops::convert (temp.getWritePointer (chan), buffer.getReadPointer (chan), numSamples); });

    return toMemory (temp);
}

template < typename SampleType >
String bufferToString (const AudioBuffer< SampleType >& buffer)
{
    return toMemory (buffer).toBase64Encoding();
}
template String bufferToString (const AudioBuffer< float >&);
template String bufferToString (const AudioBuffer< double >&);


template <>
juce::var toVar (AudioBuffer< float >& buffer)
{
    return bufferToString (buffer);
}

template <>
juce::var toVar (AudioBuffer< double >& buffer)
{
    return bufferToString (buffer);
}


void stringToBuffer (const String& string, AudioBuffer< float >& buffer)
{
    juce::MemoryBlock mem;

    if (mem.fromBase64Encoding (string))
    {
        juce::MemoryInputStream in {mem.getData(), mem.getSize(), false};
        juce::FlacAudioFormat   format;

        if (auto* reader = format.createReaderFor (&in, false))
        {
            const auto numChannels = static_cast< int > (reader->numChannels);
            const auto numSamples  = static_cast< int > (reader->lengthInSamples);

            buffer.setSize (numChannels, numSamples);
            reader->read (&buffer, 0, numSamples, 0, true, numChannels > 1);
        }
    }
}

void stringToBuffer (const String& string, AudioBuffer< double >& buffer)
{
    const auto numSamples = buffer.getNumSamples();

    AudioBuffer< float > temp {numSamples, buffer.getNumChannels()};

    bufferChannelLoop (temp,
                       [&] (int chan)
                       { vecops::convert (temp.getWritePointer (chan), buffer.getReadPointer (chan), numSamples); });

    stringToBuffer (string, temp);

    bufferChannelLoop (temp,
                       [&] (int chan)
                       { vecops::convert (buffer.getWritePointer (chan), temp.getReadPointer (chan), numSamples); });
}


template <>
AudioBuffer< float > fromVar (juce::var var)
{
    AudioBuffer< float > buffer;
    stringToBuffer (var.toString(), buffer);
    return buffer;
}

template <>
AudioBuffer< double > fromVar (juce::var var)
{
    AudioBuffer< double > buffer;
    stringToBuffer (var.toString(), buffer);
    return buffer;
}


}  // namespace lemons::serializing
