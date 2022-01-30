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
 * ======================================================================================
 */

namespace lemons::plugin
{

PluginMetadata::PluginMetadata (const ProcessorAttributes& processorAttributesToUse,
								const ParameterLayout&	   parameterLayoutToUse,
								const EditorAttributes&	   editorAttributesToUse,
								const Version&			   versionToUse)
	: processorAttributes (processorAttributesToUse), parameterLayout (parameterLayoutToUse), editorAttributes (editorAttributesToUse), version (versionToUse)
{
}


ValueTree PluginMetadata::toValueTree() const
{
	ValueTree tree { valueTreeType };

	tree.appendChild (processorAttributes.toValueTree(), nullptr);
	tree.appendChild (parameterLayout.saveToValueTree(), nullptr);
	tree.appendChild (editorAttributes.toValueTree(), nullptr);

	return tree;
}

PluginMetadata PluginMetadata::fromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (valueTreeType))
		return PluginMetadata {};

	const auto params	 = tree.getChildWithName (ParameterLayout::valueTreeType);
	const auto processor = tree.getChildWithName (ProcessorAttributes::valueTreeType);
	const auto editor	 = tree.getChildWithName (EditorAttributes::valueTreeType);

	return PluginMetadata { ProcessorAttributes::fromValueTree (processor),
							ParameterLayout::fromValueTree (params),
							EditorAttributes::fromValueTree (editor) };
}

std::unique_ptr<ProcessorBase> PluginMetadata::createProcessor() const
{
	struct TypeErasedProcessor final : public ProcessorBase
	{
		explicit TypeErasedProcessor (std::unique_ptr<dsp::Engine<float>> floatEngineToUse,
									  std::unique_ptr<dsp::Engine<double>>
										  doubleEngineToUse,
									  std::unique_ptr<State>
																 stateToUse,
									  const ProcessorAttributes& attributes,
									  const EditorAttributes&	 editorAttributes)
			: ProcessorBase (*floatEngineToUse, *doubleEngineToUse, *stateToUse, attributes), editorAttributes (editorAttributes), state (std::move (stateToUse)), floatEngine (std::move (floatEngineToUse)), doubleEngine (std::move (doubleEngineToUse))
		{
		}

	protected:

		const EditorAttributes editorAttributes;

	private:

		std::unique_ptr<State> state;

		std::unique_ptr<dsp::Engine<float>>	 floatEngine;
		std::unique_ptr<dsp::Engine<double>> doubleEngine;
	};

	return std::make_unique<TypeErasedProcessor> (dsp::factory::createEngine<float> (processorAttributes.engineType),
												  dsp::factory::createEngine<double> (processorAttributes.engineType),
												  std::make_unique<State> (parameterLayout),
												  processorAttributes, editorAttributes);
}

}  // namespace lemons::plugin


namespace lemons::files
{

template <FileType Type>
plugin::PluginMetadata loadPluginMetadata (const File& file)
{
	return plugin::PluginMetadata::fromValueTree (loadValueTree<Type> (file));
}

template plugin::PluginMetadata loadPluginMetadata<files::FileType::JSON> (const File&);
template plugin::PluginMetadata loadPluginMetadata<files::FileType::XML> (const File&);
template plugin::PluginMetadata loadPluginMetadata<files::FileType::Opaque> (const File&);


template <FileType Type>
bool savePluginMetadata (const plugin::PluginMetadata& layout, const File& file)
{
	return saveValueTree<Type> (file, layout.toValueTree());
}

template bool savePluginMetadata<files::FileType::JSON> (const plugin::PluginMetadata&, const File&);
template bool savePluginMetadata<files::FileType::XML> (const plugin::PluginMetadata&, const File&);
template bool savePluginMetadata<files::FileType::Opaque> (const plugin::PluginMetadata&, const File&);

}  // namespace lemons::files


namespace lemons::binary
{

template <files::FileType Type>
plugin::PluginMetadata getPluginMetadata (const String& filename)
{
	return plugin::PluginMetadata::fromValueTree (getValueTree<Type> (filename));
}

template plugin::PluginMetadata getPluginMetadata<files::FileType::JSON> (const String&);
template plugin::PluginMetadata getPluginMetadata<files::FileType::XML> (const String&);
template plugin::PluginMetadata getPluginMetadata<files::FileType::Opaque> (const String&);

}  // namespace lemons::binary
