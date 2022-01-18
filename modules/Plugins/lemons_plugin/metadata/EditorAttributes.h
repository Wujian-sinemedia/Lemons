#pragma once

namespace lemons::plugin
{

/** @defgroup lemons_plugin_editors Editors
    @ingroup lemons_plugin
    Plugin editor and GUI classes.
 */

/** @ingroup plugin_metadata lemons_plugin_editors
 */
struct EditorAttributes final
{
	[[nodiscard]] ValueTree toValueTree() const;

	[[nodiscard]] static EditorAttributes fromValueTree (const ValueTree& tree);

	Dimensions initialSize { Dimensions::getDefault() };

	bool isResizable { true };
	bool useResizableCorner { true };

	int msBeforeTooltip { 700 };

	static constexpr auto valueTreeType = "EditorAttributes";
};

}  // namespace lemons::plugin
