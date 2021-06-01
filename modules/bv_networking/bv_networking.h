#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                bv_networking
vendor:            Ben Vining
version:           0.0.1
name:              bv_networking
description:       Some general networking and communication utilities.
dependencies:      juce_osc

END_JUCE_MODULE_DECLARATION

#endif


#include <juce_osc/juce_osc.h>


#include "AsyncDownload/AsyncDownload.h"
#include "DownloadManager/DownloadManager.h"
