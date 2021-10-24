
namespace lemons::gui
{

void addAndMakeVisible (juce::Component* parent, juce::Component& child)
{
	jassert (parent != nullptr);
	parent->addAndMakeVisible (child);
}


void scale (juce::Component& component, const juce::Rectangle<float>& boundsRatio)
{
    // Bounds ratio cannot be empty...
    jassert (! boundsRatio.isEmpty());

	if (auto* parent = component.getParentComponent())
	{
		const auto scaledBounds = [source = parent->getLocalBounds().toFloat(), boundsRatio]() -> juce::Rectangle<float>
		{
			const auto sw = source.getWidth();
			const auto sh = source.getHeight();

			return { source.getX() + (boundsRatio.getX() * sw),
				     source.getY() + (boundsRatio.getY() * sh),
				     sw * boundsRatio.getWidth(),
				     sh * boundsRatio.getHeight() };
		}();

		const auto compBounds = scaledBounds.getSmallestIntegerContainer();

		component.setTransform (juce::AffineTransform());
		component.setBounds (compBounds);
        
        const auto transform = [source = compBounds.toFloat(), target = scaledBounds]() -> juce::AffineTransform
        { return juce::AffineTransform::translation (-source.getX(), -source.getY())
                .scaled ((target.getWidth() / source.getWidth()),
                         (target.getHeight() / source.getHeight()))
            .translated (target.getX(), target.getY()); }();

		component.setTransform (transform);
	}
    else
    {
        // You've called the scale function on a component without a parent! Assign a parent component with parent.addAndMakeVisible(yourComp) before scaling yourComp!
        jassertfalse;
    }
}

}  // namespace lemons::gui
