/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_data_model
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_data_model
 description:        Utilities for creating and managing the data model of the internal state of a plugin or app
 dependencies:       bv_core juce_audio_processors juce_data_structures juce_audio_basics
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_audio_basics/juce_audio_basics.h>

#include "bv_core/bv_core.h"


#include "valuetrees/property_nodes/nodes.h"
#include "valuetrees/property_nodes/propertyNodeGroup.h"
#include "valuetrees/Utils.h"
#include "valuetrees/json_converter.h"
#include "valuetrees/attachments/ParameterAttachments.h"
#include "valuetrees/attachments/FreestandingParameterAttachments.h"
#include "valuetrees/attachments/property-attachments/PropertyToValueTreeAttachments.h"
#include "valuetrees/attachments/property-attachments/ValueTreeToPropertyAttachments.h"
#include "valuetrees/attachments/property-attachments/PropertyAttachmentUtils.h"
