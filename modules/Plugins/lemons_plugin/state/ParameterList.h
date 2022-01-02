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

struct MetaParameterBase;


class ParameterList final
{
public:

	explicit ParameterList();

	explicit ParameterList (const ParameterLayout& layout);

	[[nodiscard]] ParameterLayout getParameterLayout() const;

	void addTo (juce::AudioProcessor& processor) const;

	void add (Parameter& parameter);

	template <typename... Args>
	void add (Parameter& first, Args&&... rest)
	{
		add (first);
		add (std::forward<Args> (rest)...);
	}

	[[nodiscard]] Parameter* getNamedParameter (const String& name) const;

	template <typename ParameterType, LEMONS_MUST_INHERIT_FROM (ParameterType, Parameter)>
	[[nodiscard]] ParameterType* getTypedParameter (const String& name) const
	{
		if (auto* param = getNamedParameter (name))
			return dynamic_cast<ParameterType*> (param);

		return nullptr;
	}

	[[nodiscard]] juce::Array<MetaParameterBase*> getMetaParameters() const;

	[[nodiscard]] juce::Array<Parameter*> getMeterParameters() const;

	void processControllerMessage (int number, int value);

	void resetAllControllerMappedParams();

	[[nodiscard]] bool isControllerMapped (int number) const;

	[[nodiscard]] ValueTree saveToValueTree() const;

	void loadFromValueTree (const ValueTree& tree);

	ToggleParameter bypass { "Bypass", false };

	static constexpr auto valueTreeType = "Parameters";


	struct Listener final
	{
		explicit Listener (const ParameterList& list,
		                   std::function<void (Parameter&)>
		                                                          onParamChange,
		                   std::function<void (Parameter&, bool)> onGestureGhange = {});

	private:
		juce::OwnedArray<ParamUpdater> updaters;
	};

private:
	std::vector<Parameter*> params;
};

}  // namespace lemons::plugin
