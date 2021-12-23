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

namespace lemons::plugin
{
/** A processor class that can simply be told the templated engine type, and takes care of instantiating a float and double one and passing them to the ProcessorBase class.
    Example usage:
    @code
    using namespace lemons::dsp;

    template<typename SampleType>
    struct MyEngine : Engine<SampleType>
    {
        // implement your audio processing here...
    };

    using MyPluginProcessor = Processor< MyEngine >;
    @endcode
    By default, this processor class does not have an editor; use the ProcessorWithEditor class to create a processor with an editor.
    @tparam EngineType A class template for your plugin's engine class. This must inherit from Engine.
    @see ProcessorBase, Engine, ProcessorWithEditor
 */
template <template <typename SampleType> class EngineType, LEMONS_MUST_INHERIT_FROM (EngineType<float>, dsp::Engine<float>)>
class Processor : public ProcessorBase
{
public:
	/** Creates a processor with the specified bus layout. */
	explicit Processor (const BusesProperties& busesLayout = getDefaultBusesLayout(),
                        const ProcessorAttributes& attributes = ProcessorAttributes::fromProjectDefines())
	    : ProcessorBase (floatEngine, doubleEngine, pluginState, busesLayout, attributes)
	{
	}

private:
	bool hasEditor() const override { return false; }

	juce::AudioProcessorEditor* createEditor() override { return nullptr; }

	EngineType<float>  floatEngine;
	EngineType<double> doubleEngine;
    
    State pluginState;
};


/** A processor class that has a GUI editor.
    Example usage:
    @code
    using namespace lemons::dsp;

    template<typename SampleType>
    struct MyEngine : Engine<SampleType>
    {
        // implement your audio processing here...
    };

    // now we can declare our plugin's GUI-less processor type:
    using HeadlessProcessor = Processor< MyEngine >;

    // now, let's define our processor's GUI:
    struct MyEditor : juce::Component
    {
        // implement your GUI here
    };

    // and now we can declare the processor with editor:
    using Plugin = ProcessorWithEditor < HeadlessProcessor, MyEditor >;
    @endcode
    @tparam ProcessorType The type of your plugin's editor-less Processor; for example, Processor<YourEngine>. This type must be a specialization of Processor.
    @tparam ComponentType The type of component for your plugin's editor to display. This type must inherit from PluginGUI.
    @see Processor, ProcessorBase, Engine
 */
template <class ProcessorType, class ComponentType, LEMONS_MUST_INHERIT_FROM (ProcessorType, ProcessorBase), LEMONS_MUST_INHERIT_FROM (ComponentType, juce::Component)>
struct ProcessorWithEditor final : ProcessorType
{
	using ProcessorType::ProcessorType;

	/** Informs the juce::AudioProcessor API that this processor supplies an editor. */
	bool hasEditor() const final { return true; }

	/** Creates an editor for this processor. */
	juce::AudioProcessorEditor* createEditor() final
	{
		return new Editor<ComponentType> (*this);
	}
};

}  // namespace lemons::dsp
