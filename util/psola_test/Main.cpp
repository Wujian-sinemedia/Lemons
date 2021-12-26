#include <lemons_plugin/lemons_plugin.h>
#include <lemons_psola/lemons_psola.h>
#include <juce_audio_processors/juce_audio_processors.h>

template<typename T>
using AudioBuffer = juce::AudioBuffer<T>;

using juce::MidiBuffer;


class PsolaTestEngine : public lemons::dsp::LatencyEngine<float>
{
    void renderChunk (const AudioBuffer<float>& input, AudioBuffer<float>& output, MidiBuffer& midiMessages, bool isBypassed) final
    {
        output.clear();
        
        if (isBypassed)
            return;
        
        auto outRight = lemons::dsp::buffers::getAliasBuffer (output, 0, output.getNumSamples(), 1, 1);
        
        osc.getSamples(outRight);
        
        analyzer.analyzeInput (outRight.getReadPointer (0), outRight.getNumSamples());

        for (const auto& m : midiMessages)
        {
            const auto message = m.getMessage();

            if (message.isNoteOn())
                shifter.setPitch (lemons::math::midiToFreq (message.getNoteNumber()));
        }
        
        auto outLeft = lemons::dsp::buffers::getAliasBuffer (output, 0, output.getNumSamples(), 1, 0);

        shifter.getSamples (outLeft);
    }
    
    void prepared (int, double samplerate, int) final
    {
        osc.setFrequency (440.f, static_cast<float>(samplerate));
        
        const auto latency = analyzer.setSamplerate (samplerate);
        
        shifter.setPitch (440);
        
        this->changeLatency (latency);
    }
    
//    void latencyChanged (int newLatency) final
//    {
//
//    }
    
    lemons::dsp::osc::Sine<float> osc;
    lemons::dsp::psola::Analyzer<float> analyzer;
    lemons::dsp::psola::Shifter<float> shifter { analyzer };
};


class PsolaTestProcessor : public lemons::plugin::BasicProcessor
{
public:
    void processBlock (AudioBuffer<float>& audio, MidiBuffer& midi) final
    {
        engine.process (audio, midi);
    }
    
    void prepareToPlay (double samplerate, int blocksize) final
    {
        engine.prepare (samplerate, blocksize, 2);
    }
    
    PsolaTestEngine engine;
};


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PsolaTestProcessor;
}
