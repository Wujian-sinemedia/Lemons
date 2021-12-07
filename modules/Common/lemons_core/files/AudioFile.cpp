namespace lemons
{

struct DefaultAudioFormats
{
	DefaultAudioFormats()
	{
		manager.registerBasicFormats();
	}

	juce::AudioFormatManager manager;
};

static DefaultAudioFormats audio_formats;

juce::AudioFormatReader* createAudioReader (const File& audioFile)
{
	return audio_formats.manager.createReaderFor (audioFile);
}

juce::AudioFormatReader* createAudioReader (std::unique_ptr<juce::InputStream> audioFileStream)
{
	return audio_formats.manager.createReaderFor (std::move (audioFileStream));
}

/*---------------------------------------------------------------------------------------------------------------------------------*/

AudioFile::AudioFile (const File& audioFile)
    : AudioFile (createAudioReader (audioFile), audioFile)
{
}

AudioFile::AudioFile (std::unique_ptr<juce::InputStream> audioStream)
    : AudioFile (createAudioReader (std::move (audioStream)), {})
{
}

AudioFile::AudioFile (juce::AudioFormatReader* reader, const juce::File& f)
    : file (f)
{
	if (reader)
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
	else
	{
		samplerate      = 0.;
		lengthInSamples = 0;
		numChannels     = 0;
		bitsPerSample   = 0;
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
	{
		if (double_data.getNumSamples() != float_data.getNumSamples()
		    || double_data.getNumChannels() != float_data.getNumChannels())
		{
			double_data.setSize (numChannels, lengthInSamples);
			double_data.clear();

			for (int chan = 0; chan < numChannels; ++chan)
				vecops::convert (double_data.getWritePointer (chan), float_data.getReadPointer (chan), lengthInSamples);
		}
	}

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
