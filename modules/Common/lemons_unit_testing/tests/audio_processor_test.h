#pragma once

#include <lemons_core/lemons_core.h>


namespace lemons::tests
{

struct AudioProcessorTestBase : public Test
{
	AudioProcessorTestBase (juce::AudioProcessor& processorToUse, const String& testName);

protected:
	void runTest() override;
    
    void fuzzParameters();
    
    template<typename SampleType>
    void prepareProcessorForPlayback (double samplerate, int blocksize);

private:
	template <typename SampleType>
	void runTypedTests();
    
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
    void runTest() final
    {
        AudioProcessorTestBase::runTest();
        
        runProductSpecificTests (processor);
    }
    
    virtual void runProductSpecificTests (ProcessorType&) { }
    
	ProcessorType processor;
};

}  // namespace lemons::tests
