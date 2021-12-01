#include <lemons_dsp/lemons_dsp.h>


namespace lemons::tests
{

AudioProcessorTestBase::AudioProcessorTestBase (juce::AudioProcessor& processorToUse, const String& testName)
    : DspTest (testName, "AudioProcessors")
    , processor (processorToUse)
{
}

void AudioProcessorTestBase::fuzzParameters()
{
    auto rand = getRandom();
    
    for (auto* parameter : processor.getParameters())
        parameter->setValueNotifyingHost (rand.nextFloat());
}

juce::AudioProcessorParameter* AudioProcessorTestBase::getNamedParameter (const String& name)
{
    for (auto* parameter : processor.getParameters())
        if (parameter->getName(50) == name)
            return parameter;
    
    return nullptr;
}

void AudioProcessorTestBase::runTest()
{
	/*
	 - accepts midi
	 - supports mpe
	 - programs API
	 - tail length secs
	 */

	beginTest ("Processor features");
    
    expect (! processor.getName().isEmpty());

	if (processor.isMidiEffect())
		expect (processor.producesMidi());
    else
    {
        juce::AudioProcessor::BusesLayout layout;
        layout.inputBuses.add (juce::AudioChannelSet::stereo());
        layout.outputBuses.add (juce::AudioChannelSet::stereo());
        
        expect (processor.checkBusesLayoutSupported (layout));
    }
    
    expect (! processor.getProgramName (processor.getCurrentProgram()).isEmpty());


	runTypedTests<float>();

	if (processor.supportsDoublePrecisionProcessing())
		runTypedTests<double>();

    
    if (! processor.getParameters().isEmpty())
        runStateTests();

	if (processor.hasEditor())
	{
#if LEMONS_GUI_UNIT_TESTS
		beginTest ("Editor tests");

		auto* editor = processor.createEditor();

		expect (editor != nullptr);

		runEditorTests (*editor);
#endif
	}
	else
	{
		expect (processor.createEditor() == nullptr);
	}
}

template <typename SampleType>
void AudioProcessorTestBase::runTypedTests()
{
    const auto precisionString = []() -> String
    {
        if constexpr (std::is_same_v<SampleType, float>)
            return "Float precision processing - ";
        else
            return "Double precision processing - ";
    }();

	AudioBuffer<SampleType> audioIO;

	for (const auto samplerate : getTestingSamplerates())
	{
		for (const auto blocksize : getTestingBlockSizes())
		{
			beginTest (precisionString + "Blocksize: " + String (blocksize) + "; Samplerate: " + String (samplerate));

			processor.enableAllBuses();

			logImportantMessage ("Prepare to play");
			processor.setRateAndBufferSizeDetails (samplerate, blocksize);
			processor.prepareToPlay (samplerate, blocksize);

			audioIO.setSize (2, blocksize, true, true, true);
			midiIO.ensureSize (static_cast<size_t> (blocksize));

            fillAudioBufferWithRandomNoise (audioIO);
            fillMidiBufferWithRandomEvents (midiIO, blocksize / 2);

			logImportantMessage ("Can call process");
			processor.processBlock (audioIO, midiIO);
            
            if (const auto latency = processor.getLatencySamples();
                latency > 0)
            {
                auto out = processor.getBusBuffer (audioIO, false, 0);
                
                const auto latencyChunk = dsp::buffers::getAliasBuffer (out, 0, latency);
                
                expect (bufferIsSilent (latencyChunk));
            }
            
			processor.processBlock (audioIO, midiIO);
            
            expect (! bufferIsSilent (audioIO));
            
			processor.processBlock (audioIO, midiIO);

			logImportantMessage ("Bypassed processing");
            
            if (auto* bypass = processor.getBypassParameter())
            {
                fillAudioBufferWithRandomNoise (audioIO);
                
                bypass->setValueNotifyingHost (1.f);
                processor.processBlock (audioIO, midiIO);
                
                auto out = processor.getBusBuffer (audioIO, false, 0);
                expect (bufferIsSilent (out));
                
                fillAudioBufferWithRandomNoise (audioIO);
                
                bypass->setValueNotifyingHost (0.f);
                processor.processBlockBypassed (audioIO, midiIO);
                
                expect (bufferIsSilent (out));
            }
            
			processor.processBlockBypassed (audioIO, midiIO);
			processor.processBlockBypassed (audioIO, midiIO);
			processor.processBlockBypassed (audioIO, midiIO);
			processor.processBlock (audioIO, midiIO);
            
            processor.reset();
            
			processor.processBlockBypassed (audioIO, midiIO);
			processor.processBlock (audioIO, midiIO);
			processor.processBlockBypassed (audioIO, midiIO);
			processor.processBlock (audioIO, midiIO);

			logImportantMessage ("Offline rendering");
			processor.setNonRealtime (true);
			processor.processBlock (audioIO, midiIO);
			processor.processBlockBypassed (audioIO, midiIO);

			processor.setNonRealtime (false);
			processor.processBlockBypassed (audioIO, midiIO);
			processor.processBlock (audioIO, midiIO);
            
            
            if (! processor.getParameters().isEmpty())
            {
                logImportantMessage ("Parameter fuzzing");
                
                fuzzParameters();
                processor.processBlock (audioIO, midiIO);
                fuzzParameters();
                processor.processBlock (audioIO, midiIO);
                fuzzParameters();
                processor.processBlock (audioIO, midiIO);
            }
		}
	}
}

template void AudioProcessorTestBase::runTypedTests<float>();
template void AudioProcessorTestBase::runTypedTests<double>();


void AudioProcessorTestBase::runStateTests()
{
    beginTest ("State saving and loading");
    
    juce::MemoryBlock block { 1024, true };
    
    struct ParamData
    {
        String name;
        float value;
    };
    
    juce::Array<ParamData> state;
    
    fuzzParameters();
    
    for (const auto* parameter : processor.getParameters())
    {
        ParamData data;
        data.name = parameter->getName(50);
        data.value = parameter->getValue();
        state.add (data);
    }
    
    processor.getStateInformation (block);
    
    fuzzParameters();
    
    processor.setStateInformation (block.getData(), static_cast<int> (block.getSize()));
    
    for (const auto& data : state)
        expectEquals (getNamedParameter (data.name)->getValue(), data.value);
    
    processor.setStateInformation (block.getData(), static_cast<int> (block.getSize()));
    processor.releaseResources();
    processor.setStateInformation (block.getData(), static_cast<int> (block.getSize()));
}

void AudioProcessorTestBase::runEditorTests (juce::AudioProcessorEditor& editor)
{
	logImportantMessage ("Correct processor");
	expect (editor.getAudioProcessor() == &processor);

	logImportantMessage ("Is resizable");
	expect (editor.isResizable());

	logImportantMessage ("Has resizable corner");
	expect (editor.resizableCorner.get() != nullptr);
}

}  // namespace lemons::tests
