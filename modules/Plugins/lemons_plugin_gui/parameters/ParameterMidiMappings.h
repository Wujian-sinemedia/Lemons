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

namespace lemons::gui::components
{

class ParameterMidiMappingEditor : public Component
{
public:

	explicit ParameterMidiMappingEditor (plugin::Parameter& parameterToUse);

	plugin::Parameter& parameter;

private:

	// is mapped
	// controller number

	// button to remove mapping
};


struct ParameterListMIDIMappingsEditor : public Component
{
public:

	explicit ParameterListMIDIMappingsEditor (plugin::ParameterList& listToUse);

	plugin::ParameterList& list;

	ComponentArray<ParameterMidiMappingEditor> parameterEditors;

	// show only mapped  /  show all
};

}  // namespace lemons::gui::components
