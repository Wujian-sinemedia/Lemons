
#pragma once

namespace ableton
{
/*!
 *  Utility class representing a bitmap in the pixelformat used by push.
 *
 *  The display uses 16 bit per pixels in a 5:6:5 format
 *
 *      MSB                             LSB
 *        b b b b|b g g g|g g g r|r r r r
 */
class PushBitmap
{
public:
    using pixel_t = uint16_t;

    static const int16_t kWidth  = 960;
    static const int16_t kHeight = 160;

    PushBitmap()
    {
        memset (data, 0, kWidth * kHeight * sizeof (pixel_t));
    }

    virtual ~PushBitmap() = default;

    int getWidth() const { return kWidth; }
    int getHeight() const { return kHeight; }

    /*!
     * \return pixel_t value in push display format from (r, g, b)
     */
    inline static pixel_t pixelFromRGB (unsigned char r, unsigned char g, unsigned char b)
    {
        pixel_t pixel = (b & 0xF8) >> 3;
        pixel <<= 6;
        pixel += (g & 0xFC) >> 2;
        pixel <<= 5;
        pixel += (r & 0xF8) >> 3;
        return pixel;
    }

    /*!
     * \return a pointer to the internal pixel data
     */
    pixel_t*       pixelData() { return data; }
    const pixel_t* pixelData() const { return data; }

private:
    pixel_t data[kWidth * kHeight]; /* static memory block to store the pixel values */
};

}  // namespace ableton
