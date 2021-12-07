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

	const Subtest beginSubtest (const String& name);

	template <typename SampleType>
	[[nodiscard]] static String getPrecisionString();

	template <typename SampleType>
	[[nodiscard]] static String getDspTestName (const String& name);

	[[nodiscard]] static const std::vector<int>    getTestingBlockSizes();
	[[nodiscard]] static const std::vector<double> getTestingSamplerates();

	[[nodiscard]] static int getNumTestingRepetitions();

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

struct DspTest : public Test
{
	DspTest (const String& testName);
};

struct MidiTest : public Test
{
	MidiTest (const String& testName);
};

}  // namespace lemons::tests

#define LEMONS_CREATE_DSP_TEST(ClassTemplate)         \
  static ClassTemplate<float>  ClassTemplate##_float; \
  static ClassTemplate<double> ClassTemplate##_double;
