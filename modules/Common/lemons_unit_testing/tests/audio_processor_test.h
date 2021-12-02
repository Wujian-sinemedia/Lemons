#pragma once

#include <lemons_core/lemons_core.h>


namespace lemons::tests
{

struct AudioProcessorTestBase : public Test
{
	AudioProcessorTestBase (juce::AudioProcessor& processorToUse, const String& testName);

private:
	void runTest() final;

	template <typename SampleType>
	void runTypedTests();
    
    void fuzzParameters();
    
	void runEditorTests (juce::AudioProcessorEditor& editor);

	juce::AudioProcessor& processor;

	MidiBuffer midiIO;
};


template <class ProcessorType, LEMONS_MUST_INHERIT_FROM (ProcessorType, juce::AudioProcessor)>
struct AudioProcessorTest : public AudioProcessorTestBase
{
	AudioProcessorTest (const String& testName)
	    : AudioProcessorTestBase (processor, testName)
	{
	}

private:
	ProcessorType processor;
};

}  // namespace lemons::tests
