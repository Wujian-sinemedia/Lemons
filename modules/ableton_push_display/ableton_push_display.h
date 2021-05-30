/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 ableton_push_display
 vendor:             Ben Vining, based on code from Ableton AG, Berlin
 version:            0.0.1
 name:               ableton_push_display
 description:       This juce module provides basic facilities for drawing to a Push 2's display using a regular juce::Component. The original example code from Ableton can be found at: https://github.com/Ableton/push2-display-with-juce.
 dependencies:   juce_gui_basics
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


#include "usb_communication/push_usb.h"
#include "push2_display/push_bitmap.h"
#include "push2_display/push_display.h"
#include "display_bridge/push_display_bridge.h"
