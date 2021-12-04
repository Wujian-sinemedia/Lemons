#include <lemons_dsp/lemons_dsp.h>


namespace lemons::tests
{

AudioProcessorTestBase::AudioProcessorTestBase (juce::AudioProcessor& processorToUse, const String& testName)
    : Test (testName, "AudioProcessors")
    , processor (processorToUse)
{
}

bool AudioProcessorTestBase::allParameterNamesAreUnique() const
{
    juce::StringArray names;
    
    for (const auto* parameter : processor->getParameters())
    {
        const auto name = parameter->getName(50);
        
        if (names.contains (name))
            return false;
        
        names.add (name);
    }
    
    return true;
}

void AudioProcessorTestBase::checkProcessorMatchesParameterState (const plugin::ProcessorHolder::ParameterState& state)
{
    for (const auto& data : state)
        expectEquals (processor.getNamedParameter (data.name)->getValue(), data.value);
}

void AudioProcessorTestBase::checkProcessorDoesNotMatchParameterState (const plugin::ProcessorHolder::ParameterState& state)
{
    for (const auto& data : state)
        expectNotEquals (processor.getNamedParameter (data.name)->getValue(), data.value);
}

void AudioProcessorTestBase::fuzzParameters()
{
    auto rand = getRandom();
    
    for (auto* parameter : processor->getParameters())
        parameter->setValueNotifyingHost (rand.nextFloat());
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
    
    expect (! processor->getName().isEmpty());
    
    expect (allParameterNamesAreUnique());

	if (processor->isMidiEffect())
		expect (processor->producesMidi());
    else
    {
        juce::AudioProcessor::BusesLayout layout;
        layout.inputBuses.add (juce::AudioChannelSet::stereo());
        layout.outputBuses.add (juce::AudioChannelSet::stereo());
        
        expect (processor->checkBusesLayoutSupported (layout));
    }
    
    beginTest ("Programs API");
    
    expect (! processor->getProgramName (processor->getCurrentProgram()).isEmpty());
    expectGreaterThan (processor->getNumPrograms(), 0);
    
    
	runTypedTests<float>();

	if (processor->supportsDoublePrecisionProcessing())
		runTypedTests<double>();

    
	if (processor->hasEditor())
	{
#if LEMONS_GUI_UNIT_TESTS
		beginTest ("Editor tests");

		auto* editor = processor->createEditor();

        expect (editor != nullptr);

		runEditorTests (*editor);
#endif
	}
	else
	{
        beginTest ("Processor should not create an editor...");
		expect (processor->createEditor() == nullptr);
	}
}

template <typename SampleType>
void AudioProcessorTestBase::runTypedTests()
{
    if constexpr (std::is_same_v<SampleType, float>)
        beginTest ("Float precision processing");
    else
        beginTest ("Double precision processing");

	AudioBuffer<SampleType> audioIO;

	for (const auto samplerate : getTestingSamplerates())
	{
        const auto samplerateSubtest = beginSubtest ("Samplerate: " + String(samplerate));
        
		for (const auto blocksize : getTestingBlockSizes())
		{
            const auto blocksizeSubtest = beginSubtest ("Blocksize: " + String(blocksize));
			
            {
                const auto subtest = beginSubtest ("Prepare to play");
                
                processor.prepareForPlayback<SampleType>(samplerate, blocksize);
                
                audioIO.setSize (2, blocksize, true, true, true);
                midiIO.ensureSize (static_cast<size_t> (blocksize));
                
                fillAudioBufferWithRandomNoise (audioIO, getRandom());
                fillMidiBufferWithRandomEvents (midiIO, blocksize / 2, getRandom());
            }
            
            {
                const auto subtest = beginSubtest ("Can call process");
                
                processor->processBlock (audioIO, midiIO);
            }

            if (const auto latency = processor->getLatencySamples();
                latency > 0)
            {
                const auto subtest = beginSubtest ("Processor has correct latency");
                
                auto out = processor->getBusBuffer (audioIO, false, 0);
                
                const auto latencyChunk = dsp::buffers::getAliasBuffer (out, 0, latency);
                
                expect (bufferIsSilent (latencyChunk));
                expectGreaterOrEqual (midiIO.getFirstEventTime(), latency);
            }
            
			processor->processBlock (audioIO, midiIO);
            
            expect (! bufferIsSilent (audioIO));
            
            for (int i = 0; i < getNumTestingRepetitions(); ++i)
                processor->processBlock (audioIO, midiIO);
            
            {
                const auto subtest = beginSubtest ("Bypassed processing");
                
                if (auto* bypass = processor->getBypassParameter())
                {
                    const auto bypassSubtest = beginSubtest ("Bypass parameter");
                    
                    fillAudioBufferWithRandomNoise (audioIO, getRandom());
                    
                    bypass->setValueNotifyingHost (1.f);
                    processor->processBlock (audioIO, midiIO);
                    
                    auto out = processor->getBusBuffer (audioIO, false, 0);
                    expect (bufferIsSilent (out));
                    
                    fillAudioBufferWithRandomNoise (audioIO, getRandom());
                    
                    bypass->setValueNotifyingHost (0.f);
                    processor->processBlockBypassed (audioIO, midiIO);
                    
                    expect (bufferIsSilent (out));
                }
                
                processor->processBlockBypassed (audioIO, midiIO);
                processor->processBlockBypassed (audioIO, midiIO);
                processor->processBlockBypassed (audioIO, midiIO);
                processor->processBlock (audioIO, midiIO);
                
                {
                    const auto st = beginSubtest ("Alternate bypassed and normal processing");
                    
                    for (int i = 0; i < getNumTestingRepetitions(); ++i)
                    {
                        processor->processBlockBypassed (audioIO, midiIO);
                        processor->processBlock (audioIO, midiIO);
                    }
                }
            }
            
            {
                const auto smallBlocksSubtest = beginSubtest ("Small blocksizes");
                
                {
                    const auto subtest = beginSubtest ("Single sample blocksize");
                    
                    midiIO.clear();
                    auto singleSample = dsp::buffers::getAliasBuffer (audioIO, 0, 1);
                    
                    processor->processBlock (singleSample, midiIO);
                    processor->processBlock (singleSample, midiIO);
                    
                    fillMidiBufferWithRandomEvents (midiIO, 1, getRandom());
                    
                    processor->processBlock (singleSample, midiIO);
                    
                    fillMidiBufferWithRandomEvents (midiIO, 12, getRandom());
                    
                    processor->processBlock (singleSample, midiIO);
                }
                {
                    const auto subtest = beginSubtest ("0 sample blocksize");
                    
                    midiIO.clear();
                    auto empty = dsp::buffers::getAliasBuffer (audioIO, 0, 0);
                    
                    processor->processBlock (empty, midiIO);
                    processor->processBlock (empty, midiIO);
                    
                    fillMidiBufferWithRandomEvents (midiIO, 1, getRandom());
                    
                    processor->processBlock (empty, midiIO);
                    
                    fillMidiBufferWithRandomEvents (midiIO, 12, getRandom());
                    
                    processor->processBlock (empty, midiIO);
                }
            }
            {
                const auto subtest = beginSubtest ("Erratic blocksize");
                
                auto rand = getRandom();
                
                for (int i = 0; i < getNumTestingRepetitions(); ++i)
                {
                    auto alias = dsp::buffers::getAliasBuffer (audioIO, 0, rand.nextInt (audioIO.getNumSamples()));
                    processor->processBlock (alias, midiIO);
                }
            }

            {
                const auto subtest = beginSubtest ("Reset() stress test");
                
                for (int i = 0; i < getNumTestingRepetitions(); ++i)
                {
                    processor->reset();
                    processor->processBlock (audioIO, midiIO);
                    processor->reset();
                    processor->processBlockBypassed (audioIO, midiIO);
                }
            }
            
            {
                const auto subtest = beginSubtest ("Offline rendering");
                
                processor->setNonRealtime (true);
                processor->processBlock (audioIO, midiIO);
                processor->processBlockBypassed (audioIO, midiIO);
                
                processor->setNonRealtime (false);
                processor->processBlockBypassed (audioIO, midiIO);
                processor->processBlock (audioIO, midiIO);
                
                for (int i = 0; i < getNumTestingRepetitions(); ++i)
                {
                    processor->setNonRealtime (true);
                    processor->processBlock (audioIO, midiIO);
                    processor->setNonRealtime (false);
                    processor->processBlock (audioIO, midiIO);
                }
            }

            if (! processor->getParameters().isEmpty())
            {
                {
                    const auto subtest = beginSubtest ("Parameter fuzzing");
                    
                    for (int i = 0; i < getNumTestingRepetitions(); ++i)
                    {
                        fuzzParameters();
                        processor->processBlock (audioIO, midiIO);
                    }
                }
                
                const auto subtest = beginSubtest ("State saving/loading");
                
                fuzzParameters();
                
                const auto prevState = processor.getStateOfParameters();
                
                audioIO.clear();
                
                processor->processBlock (audioIO, midiIO);
                
                const auto audioStorage = makeCopyOfAudioBuffer (audioIO);
                const auto midiStorage  = makeCopyOfMidiBuffer (midiIO);
                
                expect (buffersAreEqual (audioIO, audioStorage));
                expect (midiBuffersAreEqual (midiStorage, midiIO));
                
                juce::MemoryBlock block { 1024, true };
                processor->getStateInformation (block);
                
                fuzzParameters();
                
                processor->processBlock (audioIO, midiIO);
                
                checkProcessorDoesNotMatchParameterState (prevState);
                
                if (processor->isMidiEffect())
                    expect (! midiBuffersAreEqual (midiStorage, midiIO));
                else
                    expect (! buffersAreEqual (audioIO, audioStorage));
                
                processor->setStateInformation (block.getData(), static_cast<int> (block.getSize()));
                
                checkProcessorMatchesParameterState (prevState);
                
                if (processor->isMidiEffect())
                    expect (midiBuffersAreEqual (midiStorage, midiIO));
                else
                    expect (buffersAreEqual (audioIO, audioStorage));
                
                fuzzParameters();
                
                processor->releaseResources();
                processor.setStateOfParameters (prevState);
                
                checkProcessorMatchesParameterState (prevState);
            }
		}
	}
}

template void AudioProcessorTestBase::runTypedTests<float>();
template void AudioProcessorTestBase::runTypedTests<double>();


void AudioProcessorTestBase::runEditorTests (juce::AudioProcessorEditor& editor)
{
	logImportantMessage ("Correct processor");
	expect (editor.getAudioProcessor() == &processor.processor);

	logImportantMessage ("Is resizable");
	expect (editor.isResizable());

	logImportantMessage ("Has resizable corner");
	expect (editor.resizableCorner.get() != nullptr);
}

}  // namespace lemons::tests
