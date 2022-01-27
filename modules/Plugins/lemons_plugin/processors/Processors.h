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
/** @ingroup lemons_plugin_processors
	A processor class that can simply be told the templated engine type and the state type, and takes care of instantiating a float and double one and passing them to the ProcessorBase class.
	Example usage:
	@code
	using namespace lemons;

	template<typename SampleType>
	struct MyEngine : dsp::Engine<SampleType>
	{
		// implement your audio processing here...
	};

	struct MyState : plugin::State
	{
		// add your parameters here...
	};

	using MyPluginProcessor = plugin::Processor< MyEngine, MyState >;
	@endcode
	By default, this processor class does not have an editor; use the ProcessorWithEditor class to create a processor with an editor.
	@tparam EngineType A class template for your plugin's engine class. This type must inherit from Engine.
	@tparam StateType The type of the plugin's state object. This type must inherit from State.
	@see ProcessorBase, Engine, ProcessorWithEditor
 */
template <template <typename SampleType> class EngineType, typename StateType, LEMONS_MUST_INHERIT_FROM (EngineType<float>, dsp::Engine<float>), LEMONS_MUST_INHERIT_FROM (StateType, State)>
class Processor : public ProcessorBase, private locale::TranslationsInitializer
{
public:

	/** Creates a processor with the specified bus layout. */
	explicit Processor (const BusesProperties&	   busesLayout = getDefaultBusesLayout(),
						const ProcessorAttributes& attributes  = ProcessorAttributes::fromProjectDefines())
		: ProcessorBase (floatEngine, doubleEngine, pluginState, busesLayout, attributes)
	{
	}

	/** Creates a processor from the provided metadata object.
		For this constructor to work, your StateType must accept the following constructor signature:
		@code
		class StateType : public State
		{
			StateType (const ParameterLayout&);
		};
		@endcode
	*/
	explicit Processor (const PluginMetadata& metadata)
		: ProcessorBase (floatEngine, doubleEngine, pluginState, metadata.processorAttributes), pluginState (metadata.parameterLayout), editorAttributes (metadata.editorAttributes)
	{
	}

	[[nodiscard]] PluginMetadata getPluginMetadata() const
	{
		return PluginMetadata { ProcessorBase::getAttributes(), pluginState.parameters.getParameterLayout(), editorAttributes, Version::projectVersion() };
	}

protected:

	/** The plugin's state object. */
	StateType pluginState;

	const EditorAttributes editorAttributes;

private:

	EngineType<float>  floatEngine;
	EngineType<double> doubleEngine;
};


/** @ingroup lemons_plugin_processors
	A handy typedef for if you just want to create a plugin processor that wraps a certain engine type, without supplying a custom state type. */
template <template <typename SampleType> class EngineType>
using StatelessProcessor = Processor<EngineType, State>;


/** @ingroup lemons_plugin_processors
	A handy free function that creates a plugin processor wrapping the desired engine type.
 */
template <template <typename SampleType> class EngineType, typename... Args>
[[nodiscard]] std::unique_ptr<ProcessorBase> createProcessorForEngine (Args&&... args)
{
	return std::make_unique<StatelessProcessor<EngineType>> (std::forward<Args> (args)...);
}


/** @ingroup lemons_plugin_processors lemons_plugin_editors
	A processor class that has a GUI editor.
	Example usage:
	@code
	using namespace lemons;

	template<typename SampleType>
	struct MyEngine : dsp::Engine<SampleType>
	{
		// implement your audio processing here...
	};

	struct MyState : plugin::State
	{
		// add your parameters here...
	};

	// now we can declare our plugin's GUI-less processor type:
	using HeadlessProcessor = plugin::Processor< MyEngine, MyState >;

	// now, let's define our processor's GUI:
	struct MyEditor : plugin::GUI<MyState>
	{
		// implement your GUI here
	};

	// and now we can declare the processor with editor:
	using Plugin = plugin::ProcessorWithEditor < HeadlessProcessor, MyEditor >;
	@endcode
	@tparam ProcessorType The type of your plugin's editor-less Processor; for example, Processor<YourEngine>. This type must be a specialization of Processor.
	@tparam ComponentType The type of component for your plugin's editor to display. This type must inherit from PluginGUI.
	@see ProcessorWithGenericEditor, Processor, ProcessorBase, Engine
 */
template <class ProcessorType, class ComponentType, LEMONS_MUST_INHERIT_FROM (ProcessorType, ProcessorBase)>
struct ProcessorWithEditor final : ProcessorType
{
	using ProcessorType::ProcessorType;

	/** Informs the juce::AudioProcessor API that this processor supplies an editor. */
	bool hasEditor() const final { return true; }

	/** Creates an editor for this processor. */
	juce::AudioProcessorEditor* createEditor() final
	{
		return new Editor<ComponentType> (*this, this->pluginState, this->editorAttributes);
	}
};


/** @ingroup lemons_plugin_processors lemons_plugin_editors
	Handy struct that is similar to ProcessorWithEditor, but instead of providing your own component, this automatically creates a juce::GenericAudioProcessorEditor for you.
	@tparam ProcessorType The type of your plugin's editor-less Processor; for example, Processor<YourEngine>. This type must be a specialization of Processor.
	@see ProcessorWithEditor, Processor, ProcessorBase, Engine
 */
template <class ProcessorType, LEMONS_MUST_INHERIT_FROM (ProcessorType, ProcessorBase)>
struct ProcessorWithGenericEditor final : ProcessorType
{
	using ProcessorType::ProcessorType;

	/** Informs the juce::AudioProcessor API that this processor supplies an editor. */
	bool hasEditor() const final { return true; }

	/** Creates a generic editor for this processor. */
	juce::AudioProcessorEditor* createEditor() final
	{
		return new juce::GenericAudioProcessorEditor (*this);
	}
};

}  // namespace lemons::plugin


/** @ingroup lemons_plugin_processors
	A handy macro to define Juce's createPluginFilter() function for you; simply hand this macro the fully-specialized type of your plugin's processor.
 */
#define LEMONS_DEFINE_PLUGIN_CREATOR(ProcessorClass)         \
	juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() \
	{                                                        \
		return new ProcessorClass;                           \
	}

/** @ingroup lemons_plugin_processors
	A macro that creates a definition of createPluginFilter() based on a DSP engine type -- this will return a plugin that wraps the desired engine type.
 */
#define LEMONS_CREATE_PLUGIN_FROM_ENGINE(EngineType) \
	LEMONS_DEFINE_PLUGIN_CREATOR (StatelessProcessor<EngineType>)
