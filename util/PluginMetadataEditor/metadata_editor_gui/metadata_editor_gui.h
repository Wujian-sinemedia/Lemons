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

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 metadata_editor_gui
 vendor:             Lemons
 version:            0.0.1
 name:               metadata_editor_gui
 description:        GUI editor for plugin metadata
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_plugin_gui

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#include "document/Document.h"

#include "editor_attributes/EditorAttributeComponent.h"

#include "parameter_layout/ParameterLayoutComponent.h"

#include "processor_attributes/BusesLayoutComponent.h"
#include "processor_attributes/ProcessorAttributeComponent.h"

#include "document/DocumentComponent.h"
