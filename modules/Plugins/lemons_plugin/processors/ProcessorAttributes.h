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

#pragma once


namespace lemons::plugin
{

struct ProcessorAttributes
{
	[[nodiscard]] ValueTree toValueTree() const;

	void fromValueTree (const ValueTree& tree);

	[[nodiscard]] static ProcessorAttributes fromProjectDefines();


	bool acceptsMidi { true }, producesMidi { true }, supportsMPE { false }, isMidiEffect { false };

	String name;

	juce::StringArray alternateNames;

	Version version;
};

}  // namespace lemons::plugin

namespace lemons::files
{

template <FileType Type = FileType::JSON>
[[nodiscard]] plugin::ProcessorAttributes loadProcessorAttributes (const File& file);

template <FileType Type = FileType::JSON>
bool saveProcessorAttributes (const plugin::ProcessorAttributes& layout, const File& file);

}

namespace lemons::binary
{

template <files::FileType Type = files::FileType::JSON>
[[nodiscard]] plugin::ProcessorAttributes getProcessorAttributes (const String& filename);

}
