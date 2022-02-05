
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
 * ======================================================================================
 */

#include "lemons_core.h"

#include "misc/StringUtils.cpp"
#include "misc/ArgParser.cpp"
#include "misc/Dimensions.cpp"
#include "misc/Version.cpp"
#include "misc/rsa.cpp"

#include "math/Ranges.cpp"

#include "unit_tests/runner.cpp"
#include "unit_tests/test.cpp"

#include "files/FileListener.cpp"
#include "files/FileUtilities.cpp"

#include "binary_data/data.cpp"
#include "binary_data/translations.cpp"

#include "serializing/ValueTrees.cpp"
#include "serializing/VariantConversions.cpp"
#include "serializing/serializing.cpp"

#include "localization/TranslationFiles.cpp"
#include "localization/languages.cpp"

#if LEMONS_UNIT_TESTS
#	include "binary_data/BinaryData_tests.cpp"
#	include "math/Ranges_tests.cpp"
#	include "math/math_tests.cpp"
#	include "serializing/VariantConversions_tests.cpp"
#	include "serializing/serializing_tests.cpp"
#endif
