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

ProcessorAttributes ProcessorAttributes::fromProjectDefines()
{
	ProcessorAttributes attributes;

#ifdef JucePlugin_WantsMidiInput
	attributes.acceptsMidi = JucePlugin_WantsMidiInput;
#endif

#ifdef JucePlugin_ProducesMidiOutput
	attributes.producesMidi = JucePlugin_ProducesMidiOutput;
#endif

#ifdef JucePlugin_IsMidiEffect
	attributes.isMidiEffect = JucePlugin_IsMidiEffect;
#endif

#ifdef JucePlugin_Name
	attributes.name = JucePlugin_Name;
#else
	attributes.name = "AudioProcessor";
#endif

	return attributes;
}


namespace ProcessorAttributeProperties
{
static constexpr auto accepts_midi	   = "AcceptsMidi";
static constexpr auto produces_midi	   = "ProducesMidi";
static constexpr auto supports_mpe	   = "SupportsMPE";
static constexpr auto is_midi_effect   = "IsMidiEffect";
static constexpr auto processor_name   = "ProcessorName";
static constexpr auto alt_names		   = "AlternateDisplayNames";
static constexpr auto has_editor	   = "HasEditor";
static constexpr auto engine_type_name = "EngineType";
}  // namespace ProcessorAttributeProperties


ValueTree ProcessorAttributes::toValueTree() const
{
	using namespace ProcessorAttributeProperties;

	ValueTree tree { valueTreeType };

	tree.setProperty (accepts_midi, acceptsMidi, nullptr);
	tree.setProperty (produces_midi, producesMidi, nullptr);
	tree.setProperty (supports_mpe, supportsMPE, nullptr);
	tree.setProperty (is_midi_effect, isMidiEffect, nullptr);
	tree.setProperty (processor_name, name, nullptr);
	tree.setProperty (has_editor, hasEditor, nullptr);
	tree.setProperty (engine_type_name, engineType, nullptr);
	tree.setProperty (alt_names, juce::VariantConverter<juce::StringArray>::toVar (alternateNames), nullptr);

	tree.appendChild (defaultBusesLayout, nullptr);

	return tree;
}

ProcessorAttributes ProcessorAttributes::fromValueTree (const ValueTree& tree)
{
	using namespace ProcessorAttributeProperties;

	ProcessorAttributes attributes;

	if (! tree.hasType (valueTreeType))
		return attributes;

	if (tree.hasProperty (accepts_midi))
		attributes.acceptsMidi = (bool) tree.getProperty (accepts_midi);

	if (tree.hasProperty (produces_midi))
		attributes.producesMidi = (bool) tree.getProperty (produces_midi);

	if (tree.hasProperty (supports_mpe))
		attributes.supportsMPE = (bool) tree.getProperty (supports_mpe);

	if (tree.hasProperty (is_midi_effect))
		attributes.isMidiEffect = (bool) tree.getProperty (is_midi_effect);

	if (tree.hasProperty (processor_name))
		attributes.name = tree.getProperty (processor_name).toString();

	if (tree.hasProperty (has_editor))
		attributes.hasEditor = (bool) tree.getProperty (has_editor);

	if (tree.hasProperty (engine_type_name))
		attributes.engineType = tree.getProperty (engine_type_name).toString();

	if (tree.hasProperty (alt_names))
		attributes.alternateNames = juce::VariantConverter<juce::StringArray>::fromVar (tree.getProperty (alt_names));

	attributes.defaultBusesLayout = tree.getChildWithName (defaultBusesLayoutProp);

	return attributes;
}

ProcessorAttributes ProcessorAttributes::withDefaultBuses (const ValueTree& defaultBusInfo) const
{
	ProcessorAttributes attributes { *this };

	if (defaultBusInfo.hasType (defaultBusesLayoutProp))
		attributes.defaultBusesLayout = defaultBusInfo;

	return attributes;
}

}  // namespace lemons::plugin

namespace lemons::files
{

template <FileType Type>
plugin::ProcessorAttributes loadProcessorAttributes (const File& file)
{
	return plugin::ProcessorAttributes::fromValueTree (loadValueTree<Type> (file));
}

template plugin::ProcessorAttributes loadProcessorAttributes<files::FileType::JSON> (const File&);
template plugin::ProcessorAttributes loadProcessorAttributes<files::FileType::XML> (const File&);
template plugin::ProcessorAttributes loadProcessorAttributes<files::FileType::Opaque> (const File&);


template <FileType Type>
bool saveProcessorAttributes (const plugin::ProcessorAttributes& layout, const File& file)
{
	return saveValueTree<Type> (file, layout.toValueTree());
}

template bool saveProcessorAttributes<files::FileType::JSON> (const plugin::ProcessorAttributes&, const File&);
template bool saveProcessorAttributes<files::FileType::XML> (const plugin::ProcessorAttributes&, const File&);
template bool saveProcessorAttributes<files::FileType::Opaque> (const plugin::ProcessorAttributes&, const File&);

}  // namespace lemons::files


namespace lemons::binary
{

template <files::FileType Type>
plugin::ProcessorAttributes getProcessorAttributes (const String& filename)
{
	return plugin::ProcessorAttributes::fromValueTree (getValueTree<Type> (filename));
}

template plugin::ProcessorAttributes getProcessorAttributes<files::FileType::JSON> (const String&);
template plugin::ProcessorAttributes getProcessorAttributes<files::FileType::XML> (const String&);
template plugin::ProcessorAttributes getProcessorAttributes<files::FileType::Opaque> (const String&);

}  // namespace lemons::binary
