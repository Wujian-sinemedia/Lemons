/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */

namespace lemons::dsp
{

juce::AudioFormatManager& AudioFile::getFormatManager()
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

bool AudioFile::isValidAudioFile (const File& file)
{
    if (! file.existsAsFile())
        return false;
    
    return file.hasFileExtension (getFormatManager().getWildcardForAllFormats());
}

/*---------------------------------------------------------------------------------------------------------------------------------*/

AudioFile::AudioFile (const File& audioFile)
    : AudioFile (getFormatManager().createReaderFor (audioFile), audioFile)
{
}

AudioFile::AudioFile (std::unique_ptr<juce::InputStream> audioStream)
    : AudioFile (getFormatManager().createReaderFor (std::move (audioStream)), {})
{
}

AudioFile::AudioFile (juce::AudioFormatReader* reader, const juce::File& f)
    : file (f)
{
	if (reader != nullptr)
	{
		audioFormat     = reader->getFormatName();
		samplerate      = reader->sampleRate;
		lengthInSamples = static_cast<int> (reader->lengthInSamples);
		numChannels     = static_cast<int> (reader->numChannels);
		bitsPerSample   = static_cast<int> (reader->bitsPerSample);
		metadata        = reader->metadataValues;

		if (isValid())
		{
			float_data.setSize (numChannels, lengthInSamples);
			reader->read (&float_data, 0, lengthInSamples, 0, true, numChannels > 1);
		}
	}

	jassert (float_data.getNumChannels() == numChannels);
	jassert (float_data.getNumSamples() == lengthInSamples);
}

template <>
const AudioBuffer<float>& AudioFile::getData()
{
	jassert (float_data.getNumChannels() == numChannels);
	jassert (float_data.getNumSamples() == lengthInSamples);

	return float_data;
}

template <>
const AudioBuffer<double>& AudioFile::getData()
{
	if (isValid())
		if (double_data.getNumSamples() != lengthInSamples || double_data.getNumChannels() != numChannels)
            double_data.makeCopyOf (float_data);

	jassert (double_data.getNumChannels() == numChannels);
	jassert (double_data.getNumSamples() == lengthInSamples);

	return double_data;
}

double AudioFile::getLengthInSeconds() const noexcept
{
	if (samplerate > 0.)
		return static_cast<double> (lengthInSamples) / samplerate;

	return 0.;
}

bool AudioFile::isValid() const noexcept
{
	return samplerate > 0. && lengthInSamples > 0 && numChannels > 0;
}

bool AudioFile::existsOnDisk() const noexcept
{
	return file.existsAsFile();
}

double AudioFile::getSamplerate() const noexcept
{
	return samplerate;
}

int AudioFile::getNumSamples() const noexcept
{
	return lengthInSamples;
}

int AudioFile::getNumChannels() const noexcept
{
	return numChannels;
}

int AudioFile::getBitsPerSample() const noexcept
{
	return bitsPerSample;
}

File AudioFile::getFile() const noexcept
{
	return file;
}

String AudioFile::getFormatName() const noexcept
{
	return audioFormat;
}

const juce::StringPairArray& AudioFile::getMetadata() const noexcept
{
	return metadata;
}

}  // namespace lemons


/*---------------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

dsp::AudioFile getAudio (const String& audioFileName)
{
    dsp::AudioFile audio { std::make_unique<juce::MemoryInputStream> (getBlob (audioFileName), false) };
    
    jassert (audio.isValid());
    
    return audio;
}

}
