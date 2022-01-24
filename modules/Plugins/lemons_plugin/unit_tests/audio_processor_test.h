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

namespace lemons::tests
{

using juce::MidiBuffer;

/** @defgroup lemons_plugin_unit_tests Plugin tests
	@ingroup lemons_unit_testing lemons_plugin
	Utilities for unit testing plugin classes.
 */

/** @ingroup lemons_plugin_unit_tests
 */
struct AudioProcessorTestBase : public Test
{
	explicit AudioProcessorTestBase (juce::AudioProcessor& processorToUse, const String& testName);

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


/** @ingroup lemons_plugin_unit_tests
 */
template <class ProcessorType, LEMONS_MUST_INHERIT_FROM (ProcessorType, juce::AudioProcessor)>
struct AudioProcessorTest : public AudioProcessorTestBase
{
	template <typename... Args>
	explicit AudioProcessorTest (const String& testName, Args&&... args)
		: AudioProcessorTestBase (processor, testName), processor (std::forward<Args> (args)...)
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
