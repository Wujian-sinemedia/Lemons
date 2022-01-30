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
 *  ======================================================================================
 */

namespace lemons::plugin
{

namespace EditorAttributeVT
{
static constexpr auto initSize		  = "initial_size";
static constexpr auto resizable		  = "is_resizable";
static constexpr auto resizableCorner = "use_resizing_corner";
static constexpr auto tooltipTime	  = "ms_before_tooltip";
}  // namespace EditorAttributeVT

ValueTree EditorAttributes::toValueTree() const
{
	using namespace EditorAttributeVT;

	ValueTree tree { valueTreeType };

	tree.setProperty (initSize, initialSize.toString(), nullptr);
	tree.setProperty (resizable, isResizable, nullptr);
	tree.setProperty (resizableCorner, useResizableCorner, nullptr);
	tree.setProperty (tooltipTime, msBeforeTooltip, nullptr);

	return tree;
}

EditorAttributes EditorAttributes::fromValueTree (const ValueTree& tree)
{
	EditorAttributes attributes;

	using namespace EditorAttributeVT;

	if (! tree.hasType (valueTreeType))
		return attributes;

	if (tree.hasProperty (initSize))
		attributes.initialSize = Dimensions::fromString (tree.getProperty (initSize).toString());

	if (tree.hasProperty (resizable))
		attributes.isResizable = (bool) tree.getProperty (resizable);  // NOLINT

	if (tree.hasProperty (resizableCorner))
		attributes.useResizableCorner = (bool) tree.getProperty (resizableCorner);	// NOLINT

	if (tree.hasProperty (tooltipTime))
		attributes.msBeforeTooltip = (int) tree.getProperty (tooltipTime);	// NOLINT

	return attributes;
}

}  // namespace lemons::plugin
