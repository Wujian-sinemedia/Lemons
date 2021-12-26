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
#endif

	attributes.version = Version::projectVersion();

	return attributes;
}


namespace ProcessorAttributeProperties
{
static constexpr auto valueTreeType  = "ProcessorAttributes";
static constexpr auto accepts_midi   = "AcceptsMidi";
static constexpr auto produces_midi  = "ProducesMidi";
static constexpr auto supports_mpe   = "SupportsMPE";
static constexpr auto is_midi_effect = "IsMidiEffect";
static constexpr auto processor_name = "ProcessorName";
static constexpr auto alt_names      = "AlternateDisplayNames";
static constexpr auto version_prop   = "Version";
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
	tree.setProperty (alt_names, juce::VariantConverter<juce::StringArray>::toVar (alternateNames), nullptr);
	tree.setProperty (version_prop, version.toString(), nullptr);

	return tree;
}

void ProcessorAttributes::fromValueTree (const ValueTree& tree)
{
	using namespace ProcessorAttributeProperties;

	if (! tree.hasType (valueTreeType))
		return;

	if (tree.hasProperty (accepts_midi))
		acceptsMidi = (bool) tree.getProperty (accepts_midi);

	if (tree.hasProperty (produces_midi))
		producesMidi = (bool) tree.getProperty (produces_midi);

	if (tree.hasProperty (supports_mpe))
		supportsMPE = (bool) tree.getProperty (supports_mpe);

	if (tree.hasProperty (is_midi_effect))
		isMidiEffect = (bool) tree.getProperty (is_midi_effect);

	if (tree.hasProperty (processor_name))
		name = tree.getProperty (processor_name).toString();

	if (tree.hasProperty (alt_names))
		alternateNames = juce::VariantConverter<juce::StringArray>::fromVar (tree.getProperty (alt_names));

	if (tree.hasProperty (version_prop))
		version = Version::fromString (tree.getProperty (version_prop).toString());
}

template <files::FileType Type>
ProcessorAttributes fromFile (const juce::File& file)
{
	ProcessorAttributes attributes;

	attributes.fromValueTree (files::loadValueTree<Type> (file));

	return attributes;
}

template ProcessorAttributes fromFile<files::FileType::XML> (const juce::File&);
template ProcessorAttributes fromFile<files::FileType::JSON> (const juce::File&);
template ProcessorAttributes fromFile<files::FileType::Opaque> (const juce::File&);

}  // namespace lemons::plugin
