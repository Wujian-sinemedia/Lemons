namespace lemons::dsp::formats
{

juce::AudioFormatManager& getDefaultAudioFormatManager()
{
    struct DefaultAudioFormats : public juce::AudioFormatManager
    {
        DefaultAudioFormats()
        {
            this->registerBasicFormats();
        }
    };
    
    static DefaultAudioFormats manager;
    
    return manager;
}

juce::AudioFormat* getNamedFormat (const String& name)
{
    for (auto* format : getDefaultAudioFormatManager())
        if (format->getFormatName() == name)
            return format;
    
    return nullptr;
}

juce::StringArray getValidFormatNames()
{
    juce::StringArray names;
    
    for (const auto* format : getDefaultAudioFormatManager())
        names.add (format->getFormatName());
    
    return names;
}


bool isValidAudioFile (const File& file)
{
    if (! file.existsAsFile())
        return false;
    
    return file.hasFileExtension (getDefaultAudioFormatManager().getWildcardForAllFormats());
}


template <>
bool writeAudioToBlock (MemoryBlock& outputBlock,
                                   const AudioBuffer<float>& buffer, double samplerate,
                                   juce::AudioFormat& format)
{
    jassert (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0);
    
    juce::MemoryOutputStream stream { outputBlock, false };
    
    if (auto* writer = format.createWriterFor (&stream, samplerate, static_cast<unsigned> (buffer.getNumChannels()), 24, {}, 0))
    {
        return writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
    }
    
    return false;
}

template <>
bool writeAudioToBlock (MemoryBlock& outputBlock,
                                   const AudioBuffer<double>& buffer, double samplerate,
                                   juce::AudioFormat& format)
{
    AudioBuffer<float> floatBuf;
    
    floatBuf.makeCopyOf (buffer);
    
    return writeAudioToBlock (outputBlock, floatBuf, samplerate, format);
}


template <>
bool readAudioFromBlock (AudioBuffer<float>& outputBuffer,
                         const MemoryBlock& block, juce::AudioFormat& format)
{
    juce::MemoryInputStream in { block.getData(), block.getSize(), false };
    
    if (auto* reader = format.createReaderFor (&in, false))
    {
        const auto numChannels = static_cast<int> (reader->numChannels);
        const auto numSamples  = static_cast<int> (reader->lengthInSamples);
        
        if (numChannels <= 0 || numSamples <= 0)
            return false;
        
        outputBuffer.setSize (numChannels, numSamples);
        
        reader->read (&outputBuffer, 0, numSamples, 0, true, numChannels > 1);
        
        return true;
    }
    
    return false;
}

template <>
bool readAudioFromBlock (AudioBuffer<double>& outputBuffer,
                         const MemoryBlock& block, juce::AudioFormat& format)
{
    AudioBuffer<float> floatBuf;
    
    if (! readAudioFromBlock (floatBuf, block, format))
        return false;
    
    outputBuffer.makeCopyOf (floatBuf);
    
    return true;
}


template<typename SampleType>
bool saveAudioToFile (const File& destFile,
                      const AudioBuffer<SampleType>& buffer, double samplerate,
                      juce::AudioFormat& format)
{
    MemoryBlock block;
    
    if (! writeAudioToBlock (block, buffer, samplerate, format))
        return false;
    
    return files::saveBlockToFile (block, destFile);
}
 
template bool saveAudioToFile (const File&, const AudioBuffer<float>&, double, juce::AudioFormat&);
template bool saveAudioToFile (const File&, const AudioBuffer<double>&, double, juce::AudioFormat&);


template<typename SampleType>
bool loadAudioFromFile (AudioBuffer<SampleType>& outputBuffer,
                        const File& sourceFile, juce::AudioFormat& format)
{
    return readAudioFromBlock (outputBuffer, files::loadFileAsBlock (sourceFile), format);
}

template bool loadAudioFromFile (AudioBuffer<float>&, const File&, juce::AudioFormat&);
template bool loadAudioFromFile (AudioBuffer<double>&, const File&, juce::AudioFormat&);

}

