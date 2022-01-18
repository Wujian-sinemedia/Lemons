#pragma once

namespace lemons::plugin
{

/** @ingroup plugin_metadata
 */
struct PluginMetadata final
{
	explicit PluginMetadata() = default;

	explicit PluginMetadata (const ProcessorAttributes& processorAttributesToUse,
	                         const ParameterLayout&     parameterLayoutToUse,
	                         const EditorAttributes&    editorAttributesToUse,
	                         const Version&             versionToUse = Version::projectVersion());

	[[nodiscard]] ValueTree toValueTree() const;

	[[nodiscard]] static PluginMetadata fromValueTree (const ValueTree& tree);

	[[nodiscard]] std::unique_ptr<ProcessorBase> createProcessor() const;

	ProcessorAttributes processorAttributes;
	ParameterLayout     parameterLayout;
	EditorAttributes    editorAttributes;

	Version version { Version::projectVersion() };

	static constexpr auto valueTreeType = "PluginMetadata";
};

}  // namespace lemons::plugin


namespace lemons::files
{

/** @ingroup plugin_metadata lemons_files
 */
template <FileType Type = FileType::JSON>
[[nodiscard]] plugin::PluginMetadata loadPluginMetadata (const File& file);

/** @ingroup plugin_metadata lemons_files
 */
template <FileType Type = FileType::JSON>
bool savePluginMetadata (const plugin::PluginMetadata& layout, const File& file);

}  // namespace lemons::files


namespace lemons::binary
{

/** @ingroup plugin_metadata lemons_binary
 */
template <files::FileType Type = files::FileType::JSON>
[[nodiscard]] plugin::PluginMetadata getPluginMetadata (const String& filename);

}  // namespace lemons::binary
