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

#pragma once

namespace lemons::dsp
{

class AudioFileCache final
{
public:

	[[nodiscard]] static AudioFile getFromFile (const File& file);

	[[nodiscard]] static AudioFile getFromMemory (const void* data, int dataSize);

	static void setCacheTimeout (int ms);

	static void releaseUnusedFiles();

private:

	struct Pimpl;

	AudioFileCache() = delete;

	static void addFileToCache (const AudioFile& file, const juce::int64 hashCode);

	[[nodiscard]] static AudioFile getFromHashCode (const juce::int64 hashCode);

	JUCE_DECLARE_NON_COPYABLE (AudioFileCache)
};

}  // namespace lemons::dsp
