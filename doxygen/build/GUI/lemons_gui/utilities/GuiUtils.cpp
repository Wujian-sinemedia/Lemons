
/** @ingroup utilities
 *  @{
 */

namespace lemons::gui
{
ScopedCursor::ScopedCursor (
    const juce::MouseCursor& cursor,
    juce::MouseCursor        cursorToResetTo)
    : resetCursor (std::move (cursorToResetTo))
{
    juce::Desktop::getInstance().getMainMouseSource().showMouseCursor (cursor);
}

ScopedCursor::~ScopedCursor()
{
    juce::Desktop::getInstance().getMainMouseSource().showMouseCursor (
        resetCursor);
}

ScopedWaitCursor::ScopedWaitCursor()
    : ScopedCursor (juce::MouseCursor::WaitCursor)
{
}

void addAndMakeVisible (juce::Component* parent, juce::Component& child)
{
    jassert (parent != nullptr);
    parent->addAndMakeVisible (child);
}


void scale (juce::Component& component, const juce::Rectangle< float >& boundsRatio)
{
    if (boundsRatio.isEmpty())
        return;

    if (auto* parent = component.getParentComponent())
    {
        const auto scaledBounds = [source = parent->getLocalBounds().toFloat(), boundsRatio]() -> juce::Rectangle< float >
        {
            const auto sw = source.getWidth();
            const auto sh = source.getHeight();

            return {source.getX() + (boundsRatio.getX() * sw),
                    source.getY() + (boundsRatio.getY() * sh),
                    sw * boundsRatio.getWidth(),
                    sh * boundsRatio.getHeight()};
        }();

        const auto compBounds = scaledBounds.getSmallestIntegerContainer();

        component.setTransform (juce::AffineTransform());
        component.setBounds (compBounds);

        component.setTransform ([source = compBounds.toFloat(), target = scaledBounds]() -> juce::AffineTransform
                                { return juce::AffineTransform::translation (-source.getX(), -source.getY())
                                      .scaled ((target.getWidth() / source.getWidth()),
                                               (target.getHeight() / source.getHeight()))
                                      .translated (target.getX(), target.getY()); }());
    }
}

}  // namespace lemons::gui

/** @}*/
