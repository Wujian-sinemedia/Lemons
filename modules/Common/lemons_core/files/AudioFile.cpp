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
			data.setSize (numChannels, lengthInSamples);
			reader->read (&data, 0, lengthInSamples, 0, true, numChannels > 1);
		}
	}
	else
	{
		samplerate      = 0.;
		lengthInSamples = 0;
		numChannels     = 0;
		bitsPerSample   = 0;
	}

	jassert (data.getNumChannels() == numChannels);
	jassert (data.getNumSamples() == lengthInSamples);
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

}  // namespace lemons
