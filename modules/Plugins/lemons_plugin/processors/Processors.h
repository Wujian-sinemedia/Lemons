#pragma once

namespace lemons::plugin
{
/** A processor class for a plugin that can simply be told the state type and engine type, and handles everything else for you.
    Example usage:
    @code
    using namespace lemons;

    struct MyPluginState : plugin::StateBase
    {
        // put some cool stuff in here...
    };

    template<typename SampleType>
    struct MyEngine : dsp::Engine<SampleType>
    {
        // this constructor signature is required for your engine type!
        MyEngine (MyPluginState& stateToUse);

        // implement your audio processing here...
    };

    struct MyPluginProcessor : plugin::Processor< MyPluginState, MyEngine >
    {
    };
    @endcode
    By default, this processor does not have an editor; see ProcessorWithEditor to use your processor with a GUI.
    @tparam StateType The type of your plugin's state class. This must inherit from StateBase. Your state type must be default-constructable.
    @tparam EngineType A class template for your plugin's engine class. This must inherit from dsp::Engine. The constructor for your engine must take a reference to your state type; for example:
    @code
    template<typename SampleType>
    struct YourEngine : dsp::Engine<SampleType>
    {
        YourEngine (YourStateType&);
    };
    @endcode
    @see ProcessorWithEditor, ProcessorBase, PluginState, StateBase, dsp::Engine
 */
template <class StateType, template <typename SampleType> class EngineType, LEMONS_MUST_INHERIT_FROM (StateType, StateBase), LEMONS_MUST_INHERIT_FROM (EngineType<float>, dsp::Engine<float>)>
class Processor : public ProcessorBase
{
public:
	/** Creates a processor with the specified bus layout. */
	Processor (juce::AudioProcessor::BusesProperties busesLayout = BusesProperties()
	                                                                   .withInput (TRANS ("Input"), juce::AudioChannelSet::stereo(), true)
	                                                                   .withOutput (TRANS ("Output"), juce::AudioChannelSet::stereo(), true))
	    : ProcessorBase (getState(), floatEngine, doubleEngine, busesLayout)
	{
		getState().addTo (*this);
	}

protected:
	[[nodiscard]] StateType& getState() noexcept { return state.state; }

	PluginState<StateType> state;

	using State_Type_t = StateType;

private:
	void getStateInformation (juce::MemoryBlock& block) final
	{
		serializing::toBinary (state, block);
	}

	void setStateInformation (const void* data, int size) final
	{
		serializing::fromBinary (data, size, state);

		if (auto* e = getActiveEditor())
			e->setSize (state.state.dimensions.x, state.state.dimensions.y);
	}

	EngineType<float>  floatEngine { state.state };
	EngineType<double> doubleEngine { state.state };
};


/** This typedef is handy if you want to instantiate a Processor without providing a custom state type.
    For example:
    @code
    using namespace lemons;

    template<typename SampleType>
    struct MyEngine : dsp::Engine<SampleType>
    {
        // You don't have to actually use this reference for anything,
        // but this constructor signature is required.
        MyEngine (plugin::StateBase&);
    };

    using MyProcessor = plugin::StatelessProcessor< MyEngine >;
    @endcode
 */
template <template <typename SampleType> class EngineType>
using StatelessProcessor = Processor<StateBase, EngineType>;


class GUIBase;


/** A processor class that has a GUI editor.
    Example usage:
    @code
    using namespace lemons;

    // first, let's declare our plugin's state type
    struct MyPluginState : plugin::StateBase
    {
        // put some cool stuff in here...
    };

    // next, our DSP engine
    template<typename SampleType>
    struct MyEngine : dsp::Engine<SampleType>
    {
        // this constructor signature is required for your engine type!
        MyEngine (MyPluginState& stateToUse);

        // implement your audio processing here...
    };

    // now we can declare our plugin's GUI-less processor type:
    struct HeadlessProcessor : plugin::Processor< MyPluginState, MyEngine >
    {
    };

    // now, let's define our processor's GUI:
    struct MyEditor : plugin::GUI< State >
    {
        MyEditor (plugin::PluginState< YourStateType >& pluginState);
    };

    // and now we can declare the processor with editor:
    struct Plugin : plugin::ProcessorWithEditor < HeadlessProcessor, MyEditor >
    {
    };
    @endcode
    @tparam ProcessorType The type of your plugin's editor-less Processor; for example, Processor<YourState, YourEngine>. This type must be a specialization of Processor.
    @tparam ComponentType The type of component for your plugin's editor to display. This type must inherit from GUIBase or plugin::GUI and must have the following constructor signature:
    @code
    using namespace lemons;

    struct MyEditor : plugin::GUI< State >
    {
        MyEditor (plugin::PluginState< YourStateType >& pluginState);
    };
    @endcode
    @see Processor, ProcessorBase, dsp::Engine, StateBase, GUIBase, plugin::GUI
 */
template <class ProcessorType, class ComponentType, LEMONS_MUST_INHERIT_FROM (ComponentType, GUIBase)>
struct ProcessorWithEditor : ProcessorType
{
	using Size = juce::Point<int>;

	/** Creates a processor.
	    The arguments are the initial size of the plugin's editor.
	 */
	ProcessorWithEditor (const Size& initSize = defaultWindowSize())
	    : size (initSize)
	{
		jassert (isValidGuiSize (size));

		auto& dimensions = this->getState().dimensions;

		if (! isValidGuiSize (dimensions))
			dimensions = size;
	}

	/** Informs the juce::AudioProcessor API that this processor supplies an editor. */
	bool hasEditor() const final { return true; }

	/** Creates an editor for this processor. */
	juce::AudioProcessorEditor* createEditor() final
	{
		return new PluginEditor<ComponentType, typename ProcessorType::State_Type_t> (*this, this->state);
	}

private:
	[[nodiscard]] constexpr bool isValidGuiSize (const Size& toTest) const noexcept
	{
		return toTest.x > 0 && toTest.y > 0;
	}

	const Size size;
};

}  // namespace lemons::plugin
