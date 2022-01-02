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

	template <files::FileType Type = files::FileType::JSON>
	[[nodiscard]] static ProcessorAttributes fromFile (const juce::File& file);


	bool acceptsMidi { true }, producesMidi { true }, supportsMPE { false }, isMidiEffect { false };

	String name;

	juce::StringArray alternateNames;

	Version version;
};

}  // namespace lemons::plugin
