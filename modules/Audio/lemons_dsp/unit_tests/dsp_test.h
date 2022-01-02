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

struct DspTest : public Test
{
public:
	DspTest (const String& testName);

protected:
	template <typename SampleType>
	[[nodiscard]] static String getDspTestName (const String& name);
};

}  // namespace lemons::tests

#define LEMONS_CREATE_DSP_TEST(ClassTemplate)         \
  static ClassTemplate<float>  ClassTemplate##_float; \
  static ClassTemplate<double> ClassTemplate##_double;
