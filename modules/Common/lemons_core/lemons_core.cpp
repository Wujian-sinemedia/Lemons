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


#include "lemons_core.h"


#include "math/math_tests.cpp"
#include "math/vecops.cpp"

#include "misc/Version.cpp"
#include "misc/Dimensions.cpp"
#include "misc/ArgParser.cpp"

#include "files/FileUtilities.cpp"

#include "serializing/ValueTrees.cpp"
#include "serializing/serializing.cpp"
#include "serializing/VariantConversions.cpp"
#include "serializing/VariantConversions_tests.cpp"

#include "events/AutoLock.cpp"
#include "events/DeviceRotation.cpp"
#include "events/DeviceMotion.cpp"
