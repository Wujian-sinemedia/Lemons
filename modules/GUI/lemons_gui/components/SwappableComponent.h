#pragma once

namespace lemons::gui::components
{

using juce::Component;

/** @ingroup lemons_gui_components
	A component that owns and displays another type of component, which can be dynamically destroyed and recreated as different types of components.
	This object is itself a component, similar to Popup.
 */
struct SwappableComponent : public Component
{
	/** Returns true if the owned component currently exists. */
	[[nodiscard]] bool isVisible() const;

	/** Destroys the owned component, if it exists.
		@returns True if the component existed before this function call.
	*/
	bool destroy();

	/** Creates a new owned component, which may be any kind of component. If an owned component already existed, the old one will be destroyed and a new one will be created.
		@tparam ComponentType The type of owned component that will be created. This type must inherit from juce::Component.
		@param args Optional arguments that will be forwarded to the constructor of ComponentType.
		@returns Safe pointer to the new owned component.
	*/
	template <typename ComponentType, typename... Args, LEMONS_MUST_INHERIT_FROM (ComponentType, Component)>
	Component::SafePointer<ComponentType> create (Args&&... args)
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


template <typename ComponentType, LEMONS_MUST_INHERIT_FROM (ComponentType, Component)>
struct OptionalComponent : public Component
{
	/** Returns true if the owned component currently exists. */
	[[nodiscard]] bool isVisible() const
	{
		return comp.get() != nullptr;
	}

	/** Destroys the owned component, if it exists.
	@returns True if the component existed before this function call.
	*/
	bool destroy()
	{
		if (comp.get() != nullptr)
		{
			comp.reset();
			return true;
		}

		return false;
	}

	/** Creates a new owned component, which may be any kind of component. If an owned component already existed, the old one will be destroyed and a new one will be created.
	@param args Optional arguments that will be forwarded to the constructor of ComponentType.
	@returns Safe pointer to the new owned component.
	*/
	template <typename... Args>
	Component::SafePointer<ComponentType> create (Args&&... args)
	{
		comp.reset (new ComponentType (std::forward<Args> (args)...));

		auto* ptr = comp.get();
		getTopLevelComponent()->addAndMakeVisible (ptr);
		resized();

		return { dynamic_cast<ComponentType*> (ptr) };
	}

private:

	void resized() final
	{
		if (auto* c = comp.get())
			c->setBounds (getLocalBounds());
	}

	std::unique_ptr<ComponentType> comp;
};

}  // namespace lemons::gui::components
