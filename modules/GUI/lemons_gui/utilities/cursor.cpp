
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
	juce::Desktop::getInstance().getMainMouseSource().showMouseCursor (resetCursor);
}

ScopedWaitCursor::ScopedWaitCursor()
{
	juce::MouseCursor::showWaitCursor();
}

ScopedWaitCursor::~ScopedWaitCursor()
{
	juce::MouseCursor::hideWaitCursor();
}

}  // namespace lemons::gui
