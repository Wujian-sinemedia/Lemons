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

/** Base class for a plugin's entire state.
 */
class State
{
public:
    
    State();
    
    virtual ~State() = default;

	void addTo (juce::AudioProcessor& processor) const;

	void add (Parameter& parameter);

	template <typename... Args>
	void add (Parameter& first, Args&&... rest)
	{
		add (first);
		add (std::forward<Args> (rest)...);
	}
    
    [[nodiscard]] Parameter* getNamedParameter (const String& name) const;
    
    template<typename ParameterType, LEMONS_MUST_INHERIT_FROM (ParameterType, Parameter)>
    [[nodiscard]] ParameterType* getTypedParameter (const String& name) const
    {
        if (auto* param = getNamedParameter (name))
            return dynamic_cast<ParameterType*> (param);
            
        return nullptr;
    }

	void processControllerMessage (int number, int value);
    
    void resetAllControllerMappedParams();

	[[nodiscard]] bool isControllerMapped (int number) const;

	[[nodiscard]] ValueTree saveToValueTree (bool currentProgramOnly) const;

	void loadFromValueTree (const ValueTree& tree);


	struct Listener final
	{
		explicit Listener (const State& state,
		                   std::function<void (Parameter&)>
		                                                          onParamChange,
		                   std::function<void (Parameter&, bool)> onGestureGhange = {});

	private:
		juce::OwnedArray<ParamUpdater> updaters;
	};


	Dimensions editorSize { Dimensions::getDefault() };

	ToggleParameter bypass { "Bypass", false };
    
    ProgramManager programs;

private:
    [[nodiscard]] virtual ValueTree saveCustomStateData (const String& valueTreeType, bool currentProgramOnly) const;
    
    virtual void loadCustomStateData (const ValueTree& tree);
    
    std::vector<Parameter*> params;
};

}  // namespace lemons::plugin
