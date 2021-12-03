#pragma once

#include <lemons_core/lemons_core.h>
#include <juce_audio_processors/juce_audio_processors.h>


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
    
    [[nodiscard]] juce::AudioProcessorParameter* getNamedParameter (const String& name);
    
    [[nodiscard]] const juce::AudioProcessorParameter* getNamedParameter (const String& name) const;
    
    template<typename ParameterType>
    [[nodiscard]] ParameterType* getTypedParameter (const String& name)
    {
        if (auto* p = getNamedParameter (name))
            return dynamic_cast<ParameterType*>(p);
            
        return nullptr;
    }
    
    template<typename ParameterType>
    [[nodiscard]] const ParameterType* getTypedParameter (const String& name) const
    {
        if (const auto* p = getNamedParameter (name))
            return dynamic_cast<const ParameterType*>(p);
        
        return nullptr;
    }
    
    //
    
    struct ProcessorParameterData
    {
        String name;
        float value;
    };
    
    using ProcessorParameterState = juce::Array<ProcessorParameterData>;
    
    [[nodiscard]] ProcessorParameterState getStateOfProcessorParameters() const;
    
    void checkProcessorMatchesParameterState (const ProcessorParameterState& state);

private:
	template <typename SampleType>
	void runTypedTests();
    
	void runEditorTests (juce::AudioProcessorEditor& editor);
    
    [[nodiscard]] bool allParameterNamesAreUnique() const;

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
