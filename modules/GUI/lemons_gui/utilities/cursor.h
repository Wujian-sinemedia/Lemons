#pragma once

namespace lemons::gui
{

/** An RAII mechanism for temporarily changing the mouse cursor.
    @see ScopedWaitCursor
 */
struct ScopedCursor
{
    /** Changes the mouse cursor.
        @param cursor The new cursor to display for this object's lifetime.
        @param cursorToResetTo The cursor to display when this object is destroyed.
     */
    ScopedCursor (
                  const juce::MouseCursor& cursor,
                  juce::MouseCursor        cursorToResetTo = juce::MouseCursor::NormalCursor);
    
    /** Resets the mouse cursor to the reset cursor specified when this object was created.
     */
    ~ScopedCursor();
    
private:
    const juce::MouseCursor resetCursor;
};


/** An RAII mechanism for showing the system's default "wait" or "busy" cursor, then resetting to the normal cursor after this object's destruction.
    @see ScopedCursor
 */
struct ScopedWaitCursor
{
    /** Shows the system's default "wait" or "busy" cursor.
        This will turn the system cursor to an hourglass or spinning beachball until the next time the mouse is moved, or this object is destroyed.
     
        This is handy if the message loop is about to block for a couple of seconds while busy and you want to give the user feedback about this.
     */
    ScopedWaitCursor();
    
    /** Restores the system's "normal" cursor.
     */
    ~ScopedWaitCursor();
};

}
