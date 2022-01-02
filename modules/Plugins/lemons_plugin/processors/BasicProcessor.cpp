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

namespace lemons::plugin
{

juce::AudioProcessor::BusesProperties BasicProcessor::getDefaultBusesLayout()
{
	const auto stereo = juce::AudioChannelSet::stereo();

	return BusesProperties()
	    .withInput (TRANS ("Input"), stereo, true)
	    .withOutput (TRANS ("Output"), stereo, true);
}

BasicProcessor::BasicProcessor (const BusesProperties& busesLayout)
    : AudioProcessor (busesLayout)
{
}

void BasicProcessor::processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer&)
{
	audio.clear();
}

void BasicProcessor::processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer&)
{
	audio.clear();
}

bool BasicProcessor::isBusesLayoutSupported (const BusesLayout& layout) const
{
	const auto disabled = juce::AudioChannelSet::disabled();

	return layout.getMainInputChannelSet() != disabled && layout.getMainOutputChannelSet() != disabled;
}

void BasicProcessor::callEditorMethod (std::function<void (juce::AudioProcessorEditor&)> func) const
{
	if (! hasEditor())
		return;

	juce::MessageManager::callAsync ([editor = juce::Component::SafePointer<juce::AudioProcessorEditor> (getActiveEditor()), func]
	                                 {
        if (auto* e = editor.getComponent())
            func (*e); });
}

void BasicProcessor::repaintEditor() const
{
	callEditorMethod ([&] (juce::AudioProcessorEditor& e)
	                  { e.repaint(); });
}


namespace BusPropertiesVT
{
static constexpr auto nameProp    = "bus_name";
static constexpr auto defaultProp = "default_layout";
static constexpr auto activeProp  = "active_by_default";
}  // namespace BusPropertiesVT

BasicProcessor::BusProperties BasicProcessor::busPropertiesFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (busPropertiesValueTreeType))
		return {};

	BusProperties properties;

	using namespace BusPropertiesVT;

	if (tree.hasProperty (nameProp))
		properties.busName = tree.getProperty (nameProp).toString();

	if (tree.hasProperty (defaultProp))
	{
		properties.defaultLayout = juce::VariantConverter<juce::AudioChannelSet>::fromVar (tree.getProperty (defaultProp));
	}

	if (tree.hasProperty (activeProp))
		properties.isActivatedByDefault = (bool) tree.getProperty (activeProp);

	return properties;
}

ValueTree BasicProcessor::busPropertiesToValueTree (const BusProperties& properties)
{
	ValueTree tree { busPropertiesValueTreeType };

	using namespace BusPropertiesVT;

	tree.setProperty (nameProp, properties.busName, nullptr);

	tree.setProperty (defaultProp,
	                  juce::VariantConverter<juce::AudioChannelSet>::toVar (properties.defaultLayout),
	                  nullptr);

	tree.setProperty (activeProp, properties.isActivatedByDefault, nullptr);

	return tree;
}


namespace BusesPropertiesVT
{

static constexpr auto inputBusesProp  = "input_buses";
static constexpr auto outputBusesProp = "output_buses";

}  // namespace BusesPropertiesVT


BasicProcessor::BusesProperties BasicProcessor::busesPropertiesFromValueTree (const ValueTree& tree)
{
	using PropertyArray = juce::Array<BusProperties>;

	auto busPropsArrayFromVT = [&] (const ValueTree& tree) -> PropertyArray
	{
		PropertyArray array;

		for (int i = 0; i < tree.getNumChildren(); ++i)
		{
			const auto child = tree.getChild (i);

			if (child.hasType (busPropertiesValueTreeType))
				array.add (busPropertiesFromValueTree (child));
		}

		return array;
	};

	if (! tree.hasType (busesPropertiesValueTreeType))
		return {};

	BusesProperties properties;

	using namespace BusesPropertiesVT;

	auto loadValueTree = [&] (const String& childType, PropertyArray& properties)
	{
		const auto childTree = tree.getChildWithName (childType);

		if (childTree.isValid())
			properties = busPropsArrayFromVT (childTree);
	};

	loadValueTree (inputBusesProp, properties.inputLayouts);

	if (properties.inputLayouts.isEmpty())
		return getDefaultBusesLayout();

	loadValueTree (outputBusesProp, properties.outputLayouts);

	if (properties.outputLayouts.isEmpty())
		return getDefaultBusesLayout();

	return properties;
}

ValueTree BasicProcessor::busesPropertiesToValueTree (const BusesProperties& properties)
{
	ValueTree tree { busesPropertiesValueTreeType };

	auto makeValueTree = [&] (const String& type, const juce::Array<BusProperties>& layouts)
	{
		ValueTree childTree { type };

		for (const auto& buses : layouts)
			tree.appendChild (busPropertiesToValueTree (buses), nullptr);

		tree.appendChild (childTree, nullptr);
	};

	using namespace BusesPropertiesVT;

	makeValueTree (inputBusesProp, properties.inputLayouts);
	makeValueTree (outputBusesProp, properties.outputLayouts);

	return tree;
}

}  // namespace lemons::plugin
