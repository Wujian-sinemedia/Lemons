
#pragma once

namespace ableton
{
/*!
 *  Implements a bridge between juce::Graphics and push2 display format.
 */
class PushDisplayBridge
{
public:
    PushDisplayBridge (PushDisplay& d);

    /*!
     * Access a reference to the juce::Graphics of the bridge
     * the returned object can be used with juce's drawing primitives
     */
    juce::Graphics& getGraphicsContext() { return graphics; }

    /*!
     * Tells the bridge the drawing is done and the bitmap can be sent to
     * the push display
     */
    void flip();

private:
    PushDisplay&   display;
    juce::Image    image {juce::Image::RGB, PushBitmap::kWidth, PushBitmap::kHeight, false};
    juce::Graphics graphics {image};
};

}  // namespace ableton
