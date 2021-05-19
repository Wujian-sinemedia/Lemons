/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_data_model
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_data_model
 description:        Utilities for creating and managing the data model of the internal state of a plugin or app
 dependencies:       juce_audio_utils bv_core
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "bv_core/bv_core.h"


// parameters
#include "parameters/helpers/ParameterValueConversionLambdas.h"
#include "parameters/Parameter_Base.h"
#include "parameters/Parameter_ProcessorOwned.h"
#include "parameters/Parameter_SelfOwned.h"
#include "parameters/Parameter_List.h"
#include "parameters/helpers/ParameterHelpers.h"
#include "parameters/mappings/MidiCC_Mapping.h"
#include "parameters/mappings/LFO_Mapping.h"


// value trees
#include "valuetrees/property_nodes/nodes.h"
#include "valuetrees/property_nodes/propertyNodeGroup.h"
#include "valuetrees/Utils.h"
#include "valuetrees/json_converter.h"
#include "valuetrees/attachments/ParameterAttachments.h"
#include "valuetrees/attachments/FreestandingParameterAttachments.h"
#include "valuetrees/attachments/property-attachments/PropertyToValueTreeAttachments.h"
#include "valuetrees/attachments/property-attachments/ValueTreeToPropertyAttachments.h"
#include "valuetrees/attachments/property-attachments/PropertyAttachmentUtils.h"
