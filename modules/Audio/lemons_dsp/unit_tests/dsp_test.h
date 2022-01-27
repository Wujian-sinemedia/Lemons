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


#pragma once

#include <lemons_core/lemons_core.h>


namespace lemons::tests
{

/** @defgroup lemons_dsp_unit_tests DSP tests
		@ingroup lemons_unit_testing lemons_dsp
		Utilities for unit testing audio algorithms.
 */

/** @ingroup lemons_dsp_unit_tests
 */
struct DspTest : public Test
{
public:

	explicit DspTest (const String& testName);

protected:

	template <typename SampleType>
	[[nodiscard]] static String getDspTestName (const String& name);
};

}  // namespace lemons::tests

/** @ingroup lemons_dsp_unit_tests
 */
#define LEMONS_CREATE_DSP_TEST(ClassTemplate)           \
	static ClassTemplate<float>	 ClassTemplate##_float; \
	static ClassTemplate<double> ClassTemplate##_double;
