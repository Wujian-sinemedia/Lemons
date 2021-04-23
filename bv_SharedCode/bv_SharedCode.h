/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_SharedCode
 vendor:             Ben Vining
 version:            0.0.1
 name:               Ben Vining's codebase
 description:        General utilities useful for developing plugins.
 dependencies:       juce_audio_utils juce_dsp juce_osc juce_gui_extra juce_events
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once


#ifndef __BV_SHARED_CODE_H__
#define __BV_SHARED_CODE_H__

// JUCE dependencies
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_osc/juce_osc.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>

#include "core/System.h"

#include <climits>


#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-function"
#endif


#include "core/AlignedAllocate.h"
#include "core/DataHelpers.h"
#include "core/LockFreeFifo.h"
//#include "core/FileSystemWatcher.h"

#include "math/mathHelpers.h"
#include "math/VectorOps.h"
#include "math/intOps.h"

#include "gui/gui.h"
#include "gui/Spline.h"

#include "midi/MidiFIFO.h"
#include "midi/MidiUtilities.h"

#include "dsp/oscillators.h"
#include "dsp/AudioFIFO.h"
#include "dsp/Panner.h"
#include "dsp/FX/ReorderableFxChain.h"
#include "dsp/FX/NoiseGate.h"
#include "dsp/FX/Compressor.h"
#include "dsp/FX/Limiter.h"
#include "dsp/FX/DeEsser.h"
#include "dsp/FX/Reverb.h"
#include "dsp/FIFOWrappedEngine.h"
#include "dsp/pitch-detector.h"

#include "parameters/MessageQueue.h"
#include "parameters/Parameters.h"
#include "parameters/MidiCC_Mapping.h"
#include "parameters/OSC_Mapping.h"
#include "parameters/LFO_Mapping.h"
#include "parameters/ValueTreeUtilities.h"

#include "network/AsyncDownload.h"
#include "network/DownloadManager.h"


#ifndef BV_HAS_BINARY_DATA
  #define BV_HAS_BINARY_DATA 0
#endif

#if BV_HAS_BINARY_DATA
  #include "BinaryDataHelpers.h"
#endif



namespace bav
{
    
    /*
        this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
        if the directory cannot be found for your plugin, calling this function will attempt to create it.
    */
    static inline juce::File getPresetsFolder (std::string companyName, std::string pluginName)
    {
        juce::File rootFolder;
        
#if JUCE_WINDOWS
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDocumentsDirectory);
#else
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userApplicationDataDirectory);
#if JUCE_MAC
        rootFolder = rootFolder.getChildFile ("Audio").getChildFile ("Presets");
#endif
#endif
        rootFolder = rootFolder.getChildFile (companyName).getChildFile (pluginName);
        
        if (! rootFolder.isDirectory())
            rootFolder.createDirectory(); // creates the presets folder if it doesn't already exist
        
        return rootFolder;
    }
    
    
    /* helper struct that can be used to initialize Juce's translation features in a processor constructor's initialization list */
    struct TranslationInitializer
    {
        TranslationInitializer(juce::File& translationFileToUse)
        {
            juce::LocalisedStrings::setCurrentMappings (new juce::LocalisedStrings (translationFileToUse, true)); 
        }
    };
 
    
}  // namespace



#ifdef __clang__
  #pragma clang diagnostic pop
#endif


#endif  /* ifndef __BV_SHARED_CODE_H__ */
