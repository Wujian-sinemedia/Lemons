#pragma once

namespace bav::gui
{

/* RAII mechanism for changing the cursor & resetting it later */
struct ScopedCursor
{
    ScopedCursor (
                  const juce::MouseCursor& cursor,
                  juce::MouseCursor        cursorToResetTo = juce::MouseCursor::NormalCursor)
    : resetCursor (std::move (cursorToResetTo))
    {
        juce::Desktop::getInstance().getMainMouseSource().showMouseCursor (cursor);
    }
    
    ~ScopedCursor()
    {
        juce::Desktop::getInstance().getMainMouseSource().showMouseCursor (
                                                                           resetCursor);
    }
    
private:
    const juce::MouseCursor resetCursor;
};


/* Sets the cursor to the system's default "wait" cursor, then back to the normal one */
struct ScopedWaitCursor : ScopedCursor
{
    ScopedWaitCursor()
    : ScopedCursor (juce::MouseCursor::WaitCursor)
    {
    }
};

}  // namespace
