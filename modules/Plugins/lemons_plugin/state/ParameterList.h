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

#include <lemons_core/lemons_core.h>

namespace lemons::plugin
{

struct MetaParameterBase;


class ParameterList final
{
public:

	explicit ParameterList();

	explicit ParameterList (const ParameterLayout& layout);

	void addTo (juce::AudioProcessor& processor) const;

	[[nodiscard]] ParameterLayout getParameterLayout() const;

	Parameter& add (std::unique_ptr<Parameter> parameter);

	[[nodiscard]] ToggleParameter& getBypass() const;

	[[nodiscard]] Parameter* getParameterWithName (const String& name) const;

	template <typename ParameterType, LEMONS_MUST_INHERIT_FROM (ParameterType, Parameter)>
	[[nodiscard]] ParameterType* getTypedParameterWithName (const String& name) const
	{
		if (auto* param = getParameterWithName (name))
			return dynamic_cast<ParameterType*> (param);

		return nullptr;
	}

	[[nodiscard]] Parameter* getParameterWithID (const String& paramID) const;

	template <typename ParameterType, LEMONS_MUST_INHERIT_FROM (ParameterType, Parameter)>
	[[nodiscard]] ParameterType* getTypedParameterWithID (const String& paramID) const
	{
		if (auto* param = getParameterWithID (paramID))
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

	void createAndAddBypassParameter();

	struct Holder final
	{
		explicit Holder (std::unique_ptr<Parameter> parameter);

		~Holder();

		void addToProcessor (juce::AudioProcessor& processor);

		[[nodiscard]] Parameter& getParameter() const;

	private:
		bool addedToProcessor { false };

		Parameter* param { nullptr };
	};

	juce::OwnedArray<Holder> parameters;

	ToggleParameter* bypass { nullptr };
};

}  // namespace lemons::plugin
