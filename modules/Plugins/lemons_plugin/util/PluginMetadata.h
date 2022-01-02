#pragma once

namespace lemons::plugin
{

struct PluginMetadata final
{
	explicit PluginMetadata() = default;

	explicit PluginMetadata (const ProcessorAttributes& processorAttributesToUse,
	                         const ParameterLayout&     parameterLayoutToUse,
	                         const EditorAttributes&    editorAttributesToUse);

	[[nodiscard]] ValueTree toValueTree() const;

	[[nodiscard]] static PluginMetadata fromValueTree (const ValueTree& tree);

	ProcessorAttributes processorAttributes;
	ParameterLayout     parameterLayout;
	EditorAttributes    editorAttributes;
};

}  // namespace lemons::plugin


namespace lemons::files
{

template <FileType Type = FileType::JSON>
[[nodiscard]] plugin::PluginMetadata loadPluginMetadata (const File& file);

template <FileType Type = FileType::JSON>
bool savePluginMetadata (const plugin::PluginMetadata& layout, const File& file);

}  // namespace lemons::files


namespace lemons::binary
{

template <files::FileType Type = files::FileType::JSON>
[[nodiscard]] plugin::PluginMetadata getPluginMetadata (const String& filename);

}
