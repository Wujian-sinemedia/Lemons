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

#include "misc/Version.cpp"
#include "misc/Dimensions.cpp"
#include "misc/ArgParser.cpp"
#include "misc/Ranges.cpp"
#include "misc/rsa.cpp"

#include "unit_tests/test.cpp"
#include "unit_tests/runner.cpp"

#include "files/FileUtilities.cpp"

#include "binary_data/data.cpp"
#include "binary_data/translations.cpp"

#include "serializing/ValueTrees.cpp"
#include "serializing/serializing.cpp"
#include "serializing/VariantConversions.cpp"

#if LEMONS_UNIT_TESTS
#include "math/math_tests.cpp"
#include "serializing/VariantConversions_tests.cpp"
#endif

#include "localization/languages.cpp"
#include "localization/TranslationFiles.cpp"

