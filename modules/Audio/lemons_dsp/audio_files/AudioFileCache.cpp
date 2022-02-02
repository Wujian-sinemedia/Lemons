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

using juce::int64;
using juce::ScopedLock;

struct AudioFileCache::Pimpl final : private juce::Timer, private juce::DeletedAtShutdown
{
	Pimpl() = default;

	~Pimpl() final
	{
		clearSingletonInstance();
	}

	JUCE_DECLARE_SINGLETON (AudioFileCache::Pimpl, false)

	[[nodiscard]] AudioFile getFromHashCode (const int64 hashCode) noexcept
	{
		const ScopedLock sl (lock);

		for (auto& item : files)
		{
			if (item.hashCode == hashCode)
			{
				item.lastUseTime = getCurrentTimestamp();
				return item.file;
			}
		}

		return {};
	}

	void addFileToCache (AudioFile file, const int64 hashCode)
	{
		if (! file.isValid())
			return;

		if (! isTimerRunning())
			startTimer (cacheTimeout / 3);

		const ScopedLock sl (lock);

		files.add ({ file, hashCode, getCurrentTimestamp() });
	}

	void releaseUnusedFiles()
	{
		const ScopedLock sl (lock);

		for (auto i = files.size(); --i >= 0;)
			if (files.getReference (i).file.getReferenceCount() <= 1)
				files.remove (i);
	}

	unsigned cacheTimeout = 5000;

private:

	void timerCallback() final
	{
		const auto now = getCurrentTimestamp();

		const ScopedLock sl (lock);

		for (auto i = files.size(); --i >= 0;)
		{
			auto& item = files.getReference (i);

			if (item.file.getReferenceCount() <= 1)
			{
				if (now > item.lastUseTime + cacheTimeout || now < item.lastUseTime - 1000)
					files.remove (i);
			}
			else
			{
				item.lastUseTime = now;
			}
		}

		if (files.isEmpty())
			stopTimer();
	}

	[[nodiscard]] juce::uint32 getCurrentTimestamp() const noexcept
	{
		return juce::Time::getApproximateMillisecondCounter();
	}

	struct Item final
	{
		AudioFile	 file;
		int64		 hashCode;
		juce::uint32 lastUseTime;
	};

	juce::Array<Item>	  files;
	juce::CriticalSection lock;

	JUCE_DECLARE_NON_COPYABLE (Pimpl)
};

JUCE_IMPLEMENT_SINGLETON (AudioFileCache::Pimpl)


void AudioFileCache::addFileToCache (const AudioFile& file, const juce::int64 hashCode)
{
	if (file.isValid())
		Pimpl::getInstance()->addFileToCache (file, hashCode);
}

AudioFile AudioFileCache::getFromHashCode (const int64 hashCode)
{
	if (auto* p = Pimpl::getInstanceWithoutCreating())
		return p->getFromHashCode (hashCode);

	return {};
}

AudioFile AudioFileCache::getFromFile (const File& file)
{
	const auto hashCode = file.hashCode64();
	auto	   audio	= getFromHashCode (hashCode);

	if (audio.isValid())
		return audio;

	AudioFile newFile { file };

	addFileToCache (newFile, hashCode);

	return newFile;
}

AudioFile AudioFileCache::getFromMemory (const void* data, int dataSize)
{
	const auto hashCode = static_cast<int64> ((juce::pointer_sized_int) data);
	auto	   audio	= getFromHashCode (hashCode);

	if (audio.isValid())
		return audio;

	AudioFile newFile { std::make_unique<juce::MemoryInputStream> (data, static_cast<size_t> (dataSize), false) };

	addFileToCache (newFile, hashCode);

	return newFile;
}

void AudioFileCache::setCacheTimeout (int ms)
{
	jassert (ms > 0);
	Pimpl::getInstance()->cacheTimeout = static_cast<unsigned> (ms);
}

void AudioFileCache::releaseUnusedFiles()
{
	Pimpl::getInstance()->releaseUnusedFiles();
}

}  // namespace lemons::dsp
