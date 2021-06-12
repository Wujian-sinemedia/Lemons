
#pragma once

namespace bav::dsp
{
class ProcessorBase : public BasicProcessorBase
{
public:
    ProcessorBase (ParameterList&                        parameterList,
                   Engine< float >&                      floatEngineToUse,
                   Engine< double >&                     doubleEngineToUse,
                   juce::AudioProcessor::BusesProperties busesLayout);

    virtual ~ProcessorBase() override = default;

private:
    virtual BoolParameter&         getMainBypass() const = 0;
    juce::AudioProcessorParameter* getBypassParameter() const final { return &getMainBypass(); }
    
    void getStateInformation (juce::MemoryBlock& block) final;
    void setStateInformation (const void* data, int size) final;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) final;
    
    template < typename SampleType1, typename SampleType2 >
    void prepareToPlayInternal (const double sampleRate, int samplesPerBlock,
                                Engine< SampleType1 >& activeEngine, Engine< SampleType2 >& idleEngine);
    
    void releaseResources() final;
    
    bool supportsDoublePrecisionProcessing() const final { return true; }
    
    void processBlock (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi) final;
    void processBlock (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) final;
    
    template<typename SampleType>
    struct ParameterProcessor : ParameterProcessorBase<SampleType>
    {
        ParameterProcessor (ProcessorBase& p, ParameterList& l);
        
    private:
        void renderChunk (juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi) final;
        
        ProcessorBase& processor;
    };
    
    template < typename SampleType >
    void processBlockInternal (juce::AudioBuffer< SampleType >& audio,
                               juce::MidiBuffer& midi,
                               ParameterProcessor<SampleType>& parameterProcessor);

    template < typename SampleType >
    void renderChunk (Engine< SampleType >& engine, juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi);

    template < typename SampleType >
    juce::AudioBuffer< SampleType > findSubBuffer (const AudioProcessor::BusesLayout& busLayout,
                                                   juce::AudioBuffer< SampleType >& origBuffer, bool isInput);
    
    ParameterList& parameters;

    ParameterProcessor<float>  floatParameterProcessor {*this, parameters};
    ParameterProcessor<double> doubleParameterProcessor {*this, parameters};

    Engine< float >&  floatEngine;
    Engine< double >& doubleEngine;
};

}  // namespace bav::dsp
