
#pragma once

namespace ableton
{
/*!
 *  Implements a bridge between juce::Graphics and push2 display format.
 */
class PushDisplayBridge
{
public:
    PushDisplayBridge (PushDisplayInterface& d);
    
    /*!
     * Returns the size of a standard Ableton Push 2 display.
     */
    
    int getDisplayWidth() const { return PushBitmap::kWidth; }
    int getDisplayHeight() const { return PushBitmap::kHeight; }
    juce::Rectangle<int> getDisplaySize() const { return { 0, 0, PushBitmap::kWidth, PushBitmap::kHeight }; }

    /*!
     * Access a reference to the juce::Graphics of the bridge
     * the returned object can be used with juce's drawing primitives
     */
    juce::Graphics& getGraphicsContext() { return graphics; }

    /*!
     * Tells the bridge the drawing is done and the bitmap can be sent to
     * the push display. Should be called after you've finished drawing to
     * the graphics object.
     */
    void flip();
    
    /*!
     * Draws the entire component and any children into a frame and sends it to the Push display.
     */
    void drawComponent (juce::Component& c);
    void drawComponent (juce::Component* c);

private:
    PushDisplayInterface& display;
    juce::Image           image {juce::Image::RGB, PushBitmap::kWidth, PushBitmap::kHeight, false};
    juce::Graphics        graphics {image};
};

}  // namespace ableton
