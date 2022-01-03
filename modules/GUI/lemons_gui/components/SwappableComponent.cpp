namespace lemons::gui
{

SwappableComponent::SwappableComponent()
{
	setInterceptsMouseClicks (false, true);
}

bool SwappableComponent::isVisible() const
{
	return comp.get() != nullptr;
}

bool SwappableComponent::destroy()
{
	if (comp.get() != nullptr)
	{
		comp.reset();
		return true;
	}

	return false;
}

void SwappableComponent::resized()
{
	if (comp.get() != nullptr)
		comp->setBounds (getLocalBounds());
}

}  // namespace lemons::gui
