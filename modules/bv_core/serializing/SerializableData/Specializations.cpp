
namespace bav
{

SerializableFile::SerializableFile (juce::File fileToUse, juce::Identifier dataID)
: SerializableData (dataID), file(fileToUse), fileContentsPropertyName(file.getFileName() + " file contents")
{
    jassert (! file.isDirectory());
}

void SerializableFile::toValueTree (ValueTree& t)
{
    if (file.existsAsFile())
    {
        t.setProperty (fileContentsPropertyName,
                       file.loadFileAsString(),
                       nullptr);
    }
}

void SerializableFile::fromValueTree (const ValueTree& t)
{
    if (! file.existsAsFile())
        file.create();
    
    file.replaceWithText (t.getProperty (fileContentsPropertyName));
}

/*----------------------------------------------------------------------------*/

template<typename SampleType>
AudioBufferSerializer<SampleType>::AudioBufferSerializer (juce::AudioBuffer<SampleType>& bufferToSerialize,
                                                          juce::Identifier dataID)
: SerializableData(dataID), buffer(bufferToSerialize)
{
}

juce::MemoryBlock toMemory (juce::AudioBuffer<float>& buffer)
{
    juce::MemoryBlock block;
    juce::MemoryOutputStream stream {block, false};
    juce::FlacAudioFormat format;
    auto writer = format.createWriterFor (&stream, 48000, (unsigned) buffer.getNumChannels(), 24, {}, 0);
    writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
    
    return block;
}

juce::MemoryBlock toMemory (juce::AudioBuffer<double>& buffer)
{
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();
    
    juce::AudioBuffer<float> temp {numSamples, numChannels};
    
    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (temp.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);
    
    return toMemory (temp);
}

template<typename SampleType>
String bufferToString (juce::AudioBuffer<SampleType>& buffer)
{
    return toMemory (buffer).toBase64Encoding();
}
template String bufferToString (juce::AudioBuffer<float>&);
template String bufferToString (juce::AudioBuffer<double>&);

template<typename SampleType>
void AudioBufferSerializer<SampleType>::toValueTree (ValueTree& t)
{
    t.setProperty ("Audio buffer contents",
                   bufferToString (buffer),
                   nullptr);
}

void stringToBuffer (const String& string, juce::AudioBuffer<float>& buffer)
{
    juce::MemoryBlock mem;
    
    if (mem.fromBase64Encoding (string))
    {
        juce::MemoryInputStream in {mem.getData(), mem.getSize(), false};
        juce::FlacAudioFormat format;
        auto reader = format.createReaderFor (&in, false);
        
        if (reader != nullptr)
        {
            auto numChannels = (int) reader->numChannels;
            auto numSamples = (int) reader->lengthInSamples;
            buffer.setSize (numChannels, numSamples);
            
            reader->read (&buffer, 0, numSamples, 0, true, numChannels > 1);
        }
    }
}

void stringToBuffer (const String& string, juce::AudioBuffer<double>& buffer)
{
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();
    
    juce::AudioBuffer<float> temp {numSamples, numChannels};
    
    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (temp.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);
    
    stringToBuffer (string, temp);
    
    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (buffer.getWritePointer (chan), temp.getReadPointer (chan), numSamples);
}

template<typename SampleType>
void AudioBufferSerializer<SampleType>::fromValueTree (const ValueTree& t)
{
    stringToBuffer (t.getProperty ("Audio buffer contents"),
                    buffer);
}

template struct AudioBufferSerializer<float>;
template struct AudioBufferSerializer<double>;

}
