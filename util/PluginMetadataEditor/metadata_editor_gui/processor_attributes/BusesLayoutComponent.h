
#pragma once

namespace lemons::PluginMetadataEditor::gui
{

class BusesLayoutComponent final : public juce::Component
    , private plugin::BasicProcessor
{
public:

	explicit BusesLayoutComponent (plugin::ProcessorAttributes& attributes, std::function<void()> changedCallback);

	[[nodiscard]] ValueTree getCurrentState() const;

	void setState (const ValueTree& tree);

private:

	void stateChanged();

	plugin::ProcessorAttributes& processorAttributes;

	BusesProperties currentState { busesPropertiesFromValueTree (processorAttributes.defaultBusesLayout) };

	std::function<void()> onChange { nullptr };
};

}  // namespace lemons::PluginMetadataEditor::gui
