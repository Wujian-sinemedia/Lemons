
#pragma once

namespace bav::dsp
{
class ProcessorBase : public BasicProcessorBase
{
public:
    using BasicProcessorBase::BasicProcessorBase;

    virtual ~ProcessorBase() override = default;

private:
    virtual ParameterList&    getParameterList()    = 0;
    virtual BoolParameter&    getMainBypass() const = 0;
    virtual SerializableData& getStateData()        = 0;

    virtual void renderChunk (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)  = 0;
    virtual void renderChunk (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) = 0;

    /*--------------------------------------------------------------------------------------------*/

    void getStateInformation (juce::MemoryBlock& block) final;
    void setStateInformation (const void* data, int size) final;

    void processBlock (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi) final;
    void processBlock (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) final;
    
    bool supportsDoublePrecisionProcessing() const final { return true; }

    template < typename SampleType >
    void processBlockInternal (juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi);

    struct ParameterProcessor : ParameterProcessorBase
    {
        ParameterProcessor (ProcessorBase& p, ParameterList& l);

    private:
        void renderChunk (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi) final;
        void renderChunk (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) final;

        ProcessorBase& processor;
    };

    juce::AudioProcessorParameter* getBypassParameter() const final { return &getMainBypass(); }

    ParameterProcessor parameterProcessor {*this, getParameterList()};
};

}  // namespace bav::dsp
