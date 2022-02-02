/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

namespace lemons::dsp
{

using FormatReaderPtr = std::unique_ptr<juce::AudioFormatReader>;


AudioFile::AudioFileData::AudioFileData (FormatReaderPtr reader, const File& f)
	: file (f)
{
	if (const auto* r = reader.get())
	{
		audioFormat		= r->getFormatName();
		samplerate		= r->sampleRate;
		lengthInSamples = static_cast<int> (r->lengthInSamples);
		numChannels		= static_cast<int> (r->numChannels);
		bitsPerSample	= static_cast<int> (r->bitsPerSample);
		metadata		= r->metadataValues;

		if (isValid())
		{
			float_data.setSize (numChannels, lengthInSamples);
			reader->read (&float_data, 0, lengthInSamples, 0, true, numChannels > 1);
		}
	}

	jassert (float_data.getNumChannels() == numChannels);
	jassert (float_data.getNumSamples() == lengthInSamples);
}

bool AudioFile::AudioFileData::isValid() const noexcept
{
	return samplerate > 0. && lengthInSamples > 0 && numChannels > 0;
}

template <>
const AudioBuffer<float>& AudioFile::AudioFileData::getData()
{
	jassert (float_data.getNumChannels() == numChannels);
	jassert (float_data.getNumSamples() == lengthInSamples);

	return float_data;
}

template <>
const AudioBuffer<double>& AudioFile::AudioFileData::getData()
{
	if (isValid())
		if (double_data.getNumSamples() != lengthInSamples || double_data.getNumChannels() != numChannels)
			double_data.makeCopyOf (getData<float>());

	jassert (double_data.getNumChannels() == numChannels);
	jassert (double_data.getNumSamples() == lengthInSamples);

	return double_data;
}

double AudioFile::AudioFileData::getLengthInSeconds() const noexcept
{
	if (samplerate > 0.)
		return static_cast<double> (lengthInSamples) / samplerate;

	return 0.;
}

AudioFile::DataPtr AudioFile::AudioFileData::clone() const
{
	jassert (getReferenceCount() > 0);

	auto* newData = new AudioFileData;

	newData->samplerate		 = samplerate;
	newData->lengthInSamples = lengthInSamples;
	newData->numChannels	 = numChannels;
	newData->bitsPerSample	 = bitsPerSample;
	newData->file			 = file;
	newData->audioFormat	 = audioFormat;
	newData->metadata		 = metadata;
	newData->float_data		 = float_data;
	newData->double_data	 = double_data;

	return { *newData };
}

/*----------------------------------------------------------------------------------------------------------------------*/

AudioFile::AudioFile (const File& audioFile)
	: AudioFile (FormatReaderPtr (formats::getDefaultAudioFormatManager().createReaderFor (audioFile)), audioFile)
{
}

AudioFile::AudioFile (std::unique_ptr<juce::InputStream> audioStream)
	: AudioFile (FormatReaderPtr (formats::getDefaultAudioFormatManager().createReaderFor (std::move (audioStream))), {})
{
}

AudioFile::AudioFile (FormatReaderPtr reader, const juce::File& f)
	: data (new AudioFileData (std::move (reader), f))
{
}

AudioFile::AudioFile (const AudioFile& other) noexcept
	: data (other.data)
{
}

AudioFile::AudioFile (AudioFile&& other) noexcept
	: data (std::move (other.data))
{
}

int AudioFile::getReferenceCount() const noexcept
{
	if (data == nullptr)
		return 0;

	return data->getReferenceCount();
}

bool AudioFile::duplicateIfShared()
{
	if (getReferenceCount() > 1)
	{
		data = data->clone();
		return true;
	}

	return false;
}

template <typename SampleType>
const AudioBuffer<SampleType>& AudioFile::getData()
{
	if (data == nullptr)
		jassertfalse;

	return data->getData<SampleType>();
}

template const AudioBuffer<float>&	AudioFile::getData();
template const AudioBuffer<double>& AudioFile::getData();

double AudioFile::getLengthInSeconds() const noexcept
{
	if (data == nullptr)
		return 0.;

	return data->getLengthInSeconds();
}

bool AudioFile::isValid() const noexcept
{
	if (data == nullptr)
		return false;

	return data->isValid();
}

bool AudioFile::existsOnDisk() const noexcept
{
	if (data == nullptr)
		return false;

	return data->file.existsAsFile();
}

double AudioFile::getSamplerate() const noexcept
{
	if (data == nullptr)
		return 0.;

	return data->samplerate;
}

int AudioFile::getNumSamples() const noexcept
{
	if (data == nullptr)
		return 0;

	return data->lengthInSamples;
}

int AudioFile::getNumChannels() const noexcept
{
	if (data == nullptr)
		return 0;

	return data->numChannels;
}

int AudioFile::getBitsPerSample() const noexcept
{
	if (data == nullptr)
		return 0;

	return data->bitsPerSample;
}

File AudioFile::getFile() const noexcept
{
	if (data == nullptr)
		return {};

	return data->file;
}

String AudioFile::getFormatName() const noexcept
{
	if (data == nullptr)
		return {};

	return data->audioFormat;
}

juce::AudioFormat* AudioFile::getAudioFormat() const
{
	if (data == nullptr)
		return nullptr;

	auto* format = formats::getNamedFormat (data->audioFormat);

	jassert (format != nullptr);

	return format;
}

const juce::StringPairArray& AudioFile::getMetadata() const noexcept
{
	if (data == nullptr)
		jassertfalse;

	return data->metadata;
}

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

dsp::AudioFile getAudioFile (const String& audioFileName)
{
	const Data d { audioFileName };

#if LEMONS_HAS_BINARY_DATA
	jassert (d.isValid());
#endif

	return dsp::AudioFileCache::getFromMemory (d.getData(), d.getSize());
}

juce::StringArray getAudioFileNames()
{
	const auto validFileExtensions = []
	{
		auto arr = juce::StringArray::fromTokens (dsp::formats::getDefaultAudioFormatManager().getWildcardForAllFormats(),
												  ";", "");

		for (auto& element : arr)
			element = element.fromLastOccurrenceOf (".", false, false);

		arr.removeDuplicates (true);

		return arr;
	}();

	jassert (! validFileExtensions.isEmpty());

	juce::StringArray filenames;

	for (const auto& filename : getFilenames())
	{
		const auto isAudio = [&validFileExtensions, &filename]
		{
			for (const auto& xtn : validFileExtensions)
				if (filename.endsWith (xtn))
					return true;

			return false;
		}();

		if (isAudio)
			filenames.add (filename);
	}

	return filenames;
}

}  // namespace lemons::binary

/*---------------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::files
{

bool isAudioFile (const File& file)
{
	return file.hasFileExtension (dsp::formats::getDefaultAudioFormatManager().getWildcardForAllFormats());
}

}  // namespace lemons::files
