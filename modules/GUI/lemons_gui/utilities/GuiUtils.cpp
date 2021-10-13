
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

juce::Button::ButtonState boolToButtonState (const bool isOn) noexcept
{
    if (isOn) return juce::Button::ButtonState::buttonDown;

    return juce::Button::ButtonState::buttonNormal;
}

bool buttonStateToBool (juce::Button::ButtonState state) noexcept
{
    if (state == juce::Button::ButtonState::buttonDown)
        return true;

    return false;
}


void scale (juce::Component& component, const juce::Rectangle< float >& boundsRatio)
{
    if (boundsRatio.isEmpty())
        return;

    if (auto* parent = component.getParentComponent())
    {
        const auto scaledBounds = [source = parent->getLocalBounds().toFloat(), ratio = boundsRatio]() -> juce::Rectangle< float >
        {
            return {source.getX() + (ratio.getX() * source.getWidth()),
                    source.getY() + (ratio.getY() * source.getHeight()),
                    source.getWidth() * ratio.getWidth(),
                    source.getHeight() * ratio.getHeight()};
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
