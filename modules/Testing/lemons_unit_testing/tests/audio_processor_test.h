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

#include <lemons_plugin_hosting/lemons_plugin_hosting.h>


namespace lemons::tests
{

struct AudioProcessorTestBase : public Test
{
	AudioProcessorTestBase (juce::AudioProcessor& processorToUse, const String& testName);

protected:
	using ProcessorHolder = plugin::ProcessorHolder;

	void runTest() override;

	void fuzzParameters();

	void checkProcessorMatchesParameterState (const ProcessorHolder::ParameterState& state);

	void checkProcessorDoesNotMatchParameterState (const ProcessorHolder::ParameterState& state);

private:
	template <typename SampleType>
	void runTypedTests();

	void runEditorTests (juce::AudioProcessorEditor& editor);

	[[nodiscard]] bool allParameterNamesAreUnique() const;

	ProcessorHolder processor;

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
	void runTest() final
	{
		AudioProcessorTestBase::runTest();

		runProductSpecificTests (processor);
	}

	virtual void runProductSpecificTests (ProcessorType&) { }

	ProcessorType processor;
};

}  // namespace lemons::tests
