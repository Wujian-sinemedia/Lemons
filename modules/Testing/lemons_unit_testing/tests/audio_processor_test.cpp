#include <lemons_dsp/lemons_dsp.h>


namespace lemons::tests
{

AudioProcessorTestBase::AudioProcessorTestBase (juce::AudioProcessor& processorToUse, const String& testName)
: DspTest (testName, "AudioProcessors"), processor(processorToUse)
{

}

void AudioProcessorTestBase::runTest()
{
    /*
     - name
     - buses configurations
     - accepts midi
     - produces midi
     - supports mpe
     - is midi effect
     - bypass parameter
     - programs API
     - latency
     - tail length secs
     */
    
    beginTest ("Processor features");
    
    if (processor.isMidiEffect())
        expect (processor.producesMidi());
    
    
    runTypedTests<float>();
    
    if (processor.supportsDoublePrecisionProcessing())
        runTypedTests<double>();
    
    
    beginTest ("State saving and loading");
    
    juce::MemoryBlock block { 1024, true };
    
    processor.getStateInformation (block);
    processor.setStateInformation (block.getData(), static_cast<int>(block.getSize()));
    processor.setStateInformation (block.getData(), static_cast<int>(block.getSize()));
    
    processor.releaseResources();
    processor.setStateInformation (block.getData(), static_cast<int>(block.getSize()));
    
    
    if (processor.hasEditor())
    {
        beginTest ("Editor tests");

//        auto* editor = processor.createEditor();
//
//        expect (editor != nullptr);
//
//        runEditorTests (*editor);
    }
    else
    {
        beginTest ("No editor");
        expect (processor.createEditor() == nullptr);
    }
}

template<typename SampleType>
void AudioProcessorTestBase::runTypedTests()
{
    String precisionString;
    
    if constexpr (std::is_same_v<SampleType, float>)
        precisionString = "Float precision processing - ";
    else
        precisionString = "Double precision processing - ";
    
    AudioBuffer<SampleType> audioIO;
    
    for (const auto samplerate : getTestingSamplerates())
    {
        for (const auto blocksize : getTestingBlockSizes())
        {
            beginTest (precisionString + "Blocksize: " + String(blocksize) + "; Samplerate: " + String(samplerate));
            
            logImportantMessage ("Enable all buses");
            processor.enableAllBuses();
            
            logImportantMessage ("Prepare to play");
            processor.setRateAndBufferSizeDetails (samplerate, blocksize);
            processor.prepareToPlay               (samplerate, blocksize);
            
            audioIO.setSize (2, blocksize, true, true, true);
            
            midiIO.ensureSize (static_cast<size_t>(blocksize));
            
            fillAudioBufferWithRandomNoise (audioIO);
            fillMidiBufferWithRandomEvents (midiIO, blocksize / 2);
            
            logImportantMessage ("Can call process");
            processor.processBlock (audioIO, midiIO);
            processor.processBlock (audioIO, midiIO);
            processor.processBlock (audioIO, midiIO);
            
            logImportantMessage ("Bypassed processing");
            processor.processBlockBypassed (audioIO, midiIO);
            processor.processBlock (audioIO, midiIO);
            processor.processBlockBypassed (audioIO, midiIO);
            
            logImportantMessage ("Offline rendering");
            processor.setNonRealtime (true);
            processor.processBlock (audioIO, midiIO);
            processor.processBlockBypassed (audioIO, midiIO);
            
            processor.setNonRealtime (false);
            processor.processBlockBypassed (audioIO, midiIO);
            processor.processBlock (audioIO, midiIO);
        }
    }
}

template void AudioProcessorTestBase::runTypedTests<float>();
template void AudioProcessorTestBase::runTypedTests<double>();


void AudioProcessorTestBase::runEditorTests (juce::AudioProcessorEditor& editor)
{
    logImportantMessage ("Correct processor");
    expect (editor.getAudioProcessor() == &processor);
    
    logImportantMessage ("Is resizable");
    expect (editor.isResizable());
    
    logImportantMessage ("Has resizable corner");
    expect (editor.resizableCorner.get() != nullptr);
}

}

