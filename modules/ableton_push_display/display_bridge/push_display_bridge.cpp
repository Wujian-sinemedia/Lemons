
namespace ableton
{
PushDisplayBridge::PushDisplayBridge (PushDisplay& d)
    : display (d)
{
}

void PushDisplayBridge::flip()
{
    // Create a bitmap data to access the pixel rgb values
    juce::Image::BitmapData bmData {image, juce::Image::BitmapData::readOnly};

    // Create a push display bitmap and get access to the pixel data
    PushBitmap g;
    auto*      data = g.pixelData();

    const auto sizex = std::min (g.getWidth(), image.getWidth());
    const auto sizey = std::min (g.getHeight(), image.getHeight());

    // Convert the pixels, applying the xor masking needed for the display to work
    static const uint16_t xOrMasks[2] = {0xf3e7, 0xffe7};
    
    const auto hop = g.getWidth() - sizex;

    for (int y = 0; y < sizey; y++)
    {
        for (int x = 0; x < sizex; x++)
        {
            auto       c     = bmData.getPixelColour (x, y);
            const auto pixel = PushBitmap::pixelFromRGB (c.getRed(), c.getGreen(), c.getBlue());
            *data++          = pixel ^ xOrMasks[x % 2];
        }

        data += hop;
    }

    display.flip (g);
}

void PushDisplayBridge::drawComponent (juce::Component* c)
{
    if (c != nullptr)
        drawComponent (*c);
}

void PushDisplayBridge::drawComponent (juce::Component& c)
{
    // Clear previous frame
    graphics.fillAll (juce::Colour (0xff000000));
    
    c.paintEntireComponent (graphics, false);
    
    // Once we've painted into the graphics context, the frame can be sent to the display
    flip();
}

}  // namespace ableton
