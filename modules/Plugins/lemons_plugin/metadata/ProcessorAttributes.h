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

/** @ingroup plugin_metadata lemons_plugin_processors
 */
struct ProcessorAttributes final
{
	explicit ProcessorAttributes() = default;

	[[nodiscard]] ValueTree toValueTree() const;

	[[nodiscard]] ProcessorAttributes withDefaultBuses (const ValueTree& defaultBusInfo) const;

	[[nodiscard]] static ProcessorAttributes fromValueTree (const ValueTree& tree);

	[[nodiscard]] static ProcessorAttributes fromProjectDefines();


	/** @internal */
	static constexpr auto valueTreeType = "ProcessorAttributes";

	bool acceptsMidi { true }, producesMidi { true }, supportsMPE { false }, isMidiEffect { false };

	bool hasEditor { true };

	String name;

	juce::StringArray alternateNames;

	static constexpr auto defaultBusesLayoutProp = "BusesProperties";

	ValueTree defaultBusesLayout { defaultBusesLayoutProp };

	String engineType;
};

}  // namespace lemons::plugin

namespace lemons::files
{

/** @ingroup plugin_metadata lemons_files
 */
template <FileType Type = FileType::JSON>
[[nodiscard]] plugin::ProcessorAttributes loadProcessorAttributes (const File& file);

/** @ingroup plugin_metadata lemons_files */
template <FileType Type = FileType::JSON>
bool saveProcessorAttributes (const plugin::ProcessorAttributes& layout, const File& file);


}  // namespace lemons::files

namespace lemons::binary
{

/** @ingroup plugin_metadata lemons_binary
 */
template <files::FileType Type = files::FileType::JSON>
[[nodiscard]] plugin::ProcessorAttributes getProcessorAttributes (const String& filename);

}  // namespace lemons::binary
