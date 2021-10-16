
/** @ingroup utilities
 *  @{
 */
#pragma once

#include <lemons_core/lemons_core.h>
#include <lemons_mobile_utils/lemons_mobile_utils.h>
#include <juce_gui_extra/juce_gui_extra.h>

namespace lemons::gui
{
void scale (juce::Component& component, const juce::Rectangle< float >& boundsRatio);

void addAndMakeVisible (juce::Component* parent, juce::Component& child);

template < typename... Args >
static inline void addAndMakeVisible (juce::Component* parent, juce::Component& firstChild, Args&&... rest)
{
    addAndMakeVisible (parent, firstChild);
    addAndMakeVisible (parent, std::forward< Args > (rest)...);
}


/* RAII mechanism for changing the cursor & resetting it later */
struct ScopedCursor
{
    ScopedCursor (
        const juce::MouseCursor& cursor,
        juce::MouseCursor        cursorToResetTo = juce::MouseCursor::NormalCursor);

    virtual ~ScopedCursor();

private:
    const juce::MouseCursor resetCursor;
};


/* Sets the cursor to the system's default "wait" cursor, then back to the normal one */
struct ScopedWaitCursor : ScopedCursor
{
    ScopedWaitCursor();
};


}  // namespace lemons::gui

/** @}*/
