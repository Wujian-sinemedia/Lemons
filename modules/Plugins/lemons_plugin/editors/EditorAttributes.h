#pragma once

namespace lemons::plugin
{

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
