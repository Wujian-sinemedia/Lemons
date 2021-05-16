
#include "bv_SharedCode.h"


#include "async/AsyncUtils.cpp"


#include "dsp/FIFOs/AudioFIFO.cpp"
#include "dsp/Oscillators/LFO/LFO.cpp"
#include "dsp/FIFOWrappedEngine/FIFOWrappedEngine.cpp"
#include "dsp/PitchDetector/pitch-detector.cpp"
#include "dsp/PSOLA/analysis/psola_analyzer.cpp"
#include "dsp/PSOLA/resynthesis/psola_shifter.cpp"

#include "gui/components/SynthMidiKeyboardComponent/SynthMidiKeyboardComponent.cpp"
#include "gui/components/SynthStaffDisplayComponent/SynthStaffDisplayComponent.cpp"


//#if JUCE_IOS
//  #include "motion/iOS/iosMotionManager.mm"
//#endif
