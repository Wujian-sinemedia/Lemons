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

#include <juce_core/juce_core.h>


namespace lemons::tests
{

using juce::String;


enum class Intensity
{
	Low,
	Medium,
	High
};


class Test : public juce::UnitTest
{
public:
	using juce::UnitTest::UnitTest;

	[[nodiscard]] static Intensity getTestingIntensityLevel();

	static void setGlobalTestingIntensityLevel (Intensity intensityLevel);

	[[nodiscard]] static bool testingIntensityIsLow();
	[[nodiscard]] static bool testingIntensityIsMedium();
	[[nodiscard]] static bool testingIntensityIsHigh();

protected:
	void logImportantMessage (const String& message);

	struct Subtest
	{
		explicit Subtest (const String& name, Test& t);
		~Subtest();

	private:
		Test& test;
	};

	[[nodiscard]] const Subtest beginSubtest (const String& name);

	[[nodiscard]] static int getNumTestingRepetitions();

	[[nodiscard]] static const std::vector<int>    getTestingBlockSizes();
	[[nodiscard]] static const std::vector<double> getTestingSamplerates();

	template <typename SampleType>
	[[nodiscard]] static String getPrecisionString();

	template <typename Type>
	[[nodiscard]] inline static Type toVarAndBack (const Type& orig)
	{
		using Converter = juce::VariantConverter<Type>;

		return Converter::fromVar (Converter::toVar (orig));
	}

private:
	friend struct Subtest;

	void beginSubtestInternal (const String& name);
	void endSubtest();

	static constexpr auto indentSize = 4;
	String                importantMsgIndent { "  " };

	static Intensity intensity;
};


struct CoreTest : public Test
{
	CoreTest (const String& testName);
};

}  // namespace lemons::tests
