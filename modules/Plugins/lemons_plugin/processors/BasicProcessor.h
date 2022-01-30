
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

#pragma once


namespace lemons::plugin
{

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;

using juce::MidiBuffer;

/** @defgroup lemons_plugin_processors Processors
	@ingroup lemons_plugin
	Plugin processor classes.
 */

/** @ingroup lemons_plugin_processors
	A default implementation of a basic, generic juce AudioProcessor.
	The purpose of this class is to provide sensible default implementations for all of AudioProcessor's pure virtual functions, to make it easy to inherit from this class, override only what you need, and get working.
	@see ProcessorBase
 */
class BasicProcessor : public juce::AudioProcessor
{
public:

	using BusesProperties = juce::AudioProcessor::BusesProperties;
	using BusProperties	  = juce::AudioProcessor::BusProperties;

	/** Creates a basic processor.
		The default constructor initializes an audio processor with stereo in and out buses.
	*/
	explicit BasicProcessor (const BusesProperties& busesLayout = getDefaultBusesLayout());

	/** Returns a BusesLayout with stereo in and out buses that are enabled by default. */
	[[nodiscard]] static BusesProperties getDefaultBusesLayout();

	[[nodiscard]] juce::AudioProcessorParameter* getParameter (const String& parameterName) const;

protected:

	/** @name basic_processor_editor_funcs Interacting with the editor */
	///@{

	/** Asynchronously calls a method of the current editor, if there is one, on the message thread.
		Internally, this uses the Juce MessageManager's callAsync method. Calling this on the audio thread should be avoided at all costs. I would personally only use this method in setStateInformation().
	*/
	void callEditorMethod (std::function<void (juce::AudioProcessorEditor&)> func) const;

	/** Repaints the editor, if one exists. This is a utility function that uses callEditorMethod() under the hood. Calling this on the audio thread should be avoided at all costs.
	 */
	void repaintEditor() const;

	///@}


	/** @name buses_properties_serializing Serializing BusesProperties */
	///@{

	[[nodiscard]] static BusesProperties busesPropertiesFromValueTree (const ValueTree& tree);

	[[nodiscard]] static ValueTree busesPropertiesToValueTree (const BusesProperties& properties);

	///@}

private:

	void prepareToPlay (double /* samplerate */, int /* blocksize */) override { }
	void releaseResources() override { }

	void processBlock (AudioBuffer<float>&, MidiBuffer&) override { }
	void processBlock (AudioBuffer<double>&, MidiBuffer&) override { }
	void processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer&) override;
	void processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer&) override;

	bool supportsDoublePrecisionProcessing() const override { return true; }

	double getTailLengthSeconds() const override { return 0.; }

	void getStateInformation (juce::MemoryBlock&) override { }
	void setStateInformation (const void*, int) override { }

	int			 getNumPrograms() override { return 1; }
	int			 getCurrentProgram() override { return 0; }
	void		 setCurrentProgram (int) override { }
	const String getProgramName (int) override { return TRANS ("Program"); }
	void		 changeProgramName (int, const String&) override { }

	bool acceptsMidi() const override { return true; }
	bool producesMidi() const override { return true; }
	bool supportsMPE() const override { return false; }
	bool isMidiEffect() const override { return false; }

	const String getName() const override { return TRANS ("BasicProcessor"); }

	bool						hasEditor() const override { return false; }
	juce::AudioProcessorEditor* createEditor() override { return nullptr; }

	bool isBusesLayoutSupported (const BusesLayout& layout) const override;


	[[nodiscard]] static BusProperties busPropertiesFromValueTree (const ValueTree& tree);

	[[nodiscard]] static ValueTree busPropertiesToValueTree (const BusProperties& properties);

	static constexpr auto busesPropertiesValueTreeType = "BusesProperties";
	static constexpr auto busPropertiesValueTreeType   = "BusProperties";
};


}  // namespace lemons::plugin
