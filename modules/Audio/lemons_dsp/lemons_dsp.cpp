/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#include "lemons_dsp.h"


#include "unit_tests/dsp_test.cpp"
#include "unit_tests/dsp_utils.cpp"

#include "fifos/CircularBuffer.cpp"
#include "fifos/AudioFifo.cpp"
#include "fifos/AudioAndMidiFIFO.cpp"

#include "engines/AudioEngine.cpp"
#include "engines/LatencyEngine.cpp"
#include "engines/Factory.cpp"
#include "engines/EngineChain.cpp"
#include "engines/DspProtection.cpp"

#include "filters/Coeffecients.cpp"
#include "filters/Filter.cpp"

#include "sample_streams/SampleStream.cpp"
#include "sample_streams/WhiteNoiseGenerator.cpp"
#include "sample_streams/oscillator.cpp"

#include "util/AudioFormats.cpp"
#include "util/serializing.cpp"
#include "util/ValueSmoother.cpp"
#include "util/BufferUtils.cpp"
#include "util/MonoStereoConverter.cpp"

#include "oscillators/basic_types.cpp"
#include "oscillators/Detunable.cpp"
#include "oscillators/choosable.cpp"

#include "audio_files/AudioFile.cpp"
#include "audio_files/AudioFilePlayer.cpp"

#if LEMONS_UNIT_TESTS
#  include "fifos/CircularBuffer_tests.cpp"
#  include "fifos/AudioFifo_tests.cpp"
#  include "fifos/AudioAndMidiFIFO_tests.cpp"
#  include "engines/AudioEngine_tests.cpp"
#  include "engines/LatencyEngine_tests.cpp"
#  include "oscillators/oscillator_tests.cpp"
#  include "util/BufferUtils_tests.cpp"
#  include "util/MonoStereoConverter_tests.cpp"
#  include "util/serializing_tests.cpp"
#endif
