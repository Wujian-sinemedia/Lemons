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

#pragma once

namespace lemons::PluginMetadataEditor::gui
{

class DocumentComponent final : public juce::Component
{
public:

private:

	void stateChanged();

	Document document;

	EditorAttributeComponent	editorView { document.metadata.editorAttributes, [this]
											 { this->stateChanged(); } };
	ParameterLayoutComponent	parameterView { document.metadata.parameterLayout, [this]
												{ this->stateChanged(); } };
	ProcessorAttributeComponent processorView { document.metadata.processorAttributes, [this]
												{ this->stateChanged(); } };
};

}  // namespace lemons::PluginMetadataEditor::gui
