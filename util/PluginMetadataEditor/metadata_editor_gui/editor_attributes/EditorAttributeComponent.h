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

#include <lemons_gui/lemons_gui.h>

namespace lemons::PluginMetadataEditor::gui
{

using namespace lemons::gui::components;

using namespace lemons::gui;

class EditorAttributeComponent final : public juce::Component
{
public:

	explicit EditorAttributeComponent (plugin::EditorAttributes& attributesToUse, std::function<void()> callback);

private:

	std::function<void()> stateChanged;

	plugin::EditorAttributes& attributes;

	ToggleTextButton isResizable { TRANS ("Is resizable"), attributes.isResizable, [this] (bool v)
								   { this->attributes.isResizable = v; this->stateChanged(); } };

	ToggleTextButton useResizableCorner { TRANS ("Use resizable corner"), attributes.useResizableCorner, [this] (bool v)
										  { this->attributes.useResizableCorner = v; this->stateChanged(); } };

	/*
	 initial size
	 ms before tooltip
	 */
};

}  // namespace lemons::PluginMetadataEditor::gui
