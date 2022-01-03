#pragma once

namespace lemons::gui
{

struct SwappableComponent final : public Component
{
	explicit SwappableComponent();

	[[nodiscard]] bool isVisible() const;

	bool destroy();

	template <typename ComponentType, typename... Args, LEMONS_MUST_INHERIT_FROM (ComponentType, Component)>
	juce::Component::SafePointer<ComponentType> create (Args&&... args)
	{
		comp.reset (new ComponentType (std::forward<Args> (args)...));

		auto* ptr = comp.get();
		getTopLevelComponent()->addAndMakeVisible (ptr);
		resized();

		return { dynamic_cast<ComponentType*> (ptr) };
	}

private:
	void resized() final;

	std::unique_ptr<Component> comp;
};

}  // namespace lemons::gui
