
#pragma once

namespace bav::gui
{

template<typename... Args>
static inline void addAndMakeVisible (juce::Component* parent, juce::Component* firstChild, Args... rest)
{
    addAndMakeVisible (parent, firstChild);
    addAndMakeVisible (parent, std::forward<Args>(rest)...);
}

static inline void addAndMakeVisible (juce::Component* parent, juce::Component* child)
{
    jassert (parent != nullptr && parent != nullptr);
    parent->addAndMakeVisible (child);
}

static inline juce::Button::ButtonState boolToButtonState (const bool isOn) noexcept
{
    if (isOn) return juce::Button::ButtonState::buttonDown;

    return juce::Button::ButtonState::buttonNormal;
}

static inline bool buttonStateToBool (juce::Button::ButtonState state) noexcept
{
    if (state == juce::Button::ButtonState::buttonDown)
        return true;

    return false;
}


/*
        Draws a square bevel around a given rectange.
        This is useful for insetting components and giving them a border.
     */
static void drawBevel (juce::Graphics& g,
                       juce::Rectangle< float >
                                           innerBevelBounds,
                       float               bevelThickness,
                       const juce::Colour& baseColour)
{
    juce::Rectangle< float > outerBevelBounds (
        innerBevelBounds.expanded (bevelThickness, bevelThickness));
    juce::Rectangle< float > centreBevelBounds (
        innerBevelBounds.expanded (bevelThickness * 0.5f, bevelThickness * 0.5f));

    juce::Path pL, pR, pT, pB, pTL, pTR, pBL, pBR;
    pL.startNewSubPath (centreBevelBounds.getTopLeft());
    pL.lineTo (centreBevelBounds.getBottomLeft());

    pR.startNewSubPath (centreBevelBounds.getTopRight());
    pR.lineTo (centreBevelBounds.getBottomRight());

    pT.startNewSubPath (centreBevelBounds.getTopLeft());
    pT.lineTo (centreBevelBounds.getTopRight());

    pB.startNewSubPath (centreBevelBounds.getBottomLeft());
    pB.lineTo (centreBevelBounds.getBottomRight());

    pTL.addTriangle (outerBevelBounds.getX(),
                     outerBevelBounds.getY(),
                     outerBevelBounds.getX(),
                     innerBevelBounds.getY(),
                     innerBevelBounds.getX(),
                     innerBevelBounds.getY());

    pTR.addTriangle (outerBevelBounds.getRight(),
                     outerBevelBounds.getY(),
                     outerBevelBounds.getRight(),
                     innerBevelBounds.getY(),
                     innerBevelBounds.getRight(),
                     innerBevelBounds.getY());

    pBL.addTriangle (outerBevelBounds.getX(),
                     outerBevelBounds.getBottom(),
                     outerBevelBounds.getX(),
                     innerBevelBounds.getBottom(),
                     innerBevelBounds.getX(),
                     innerBevelBounds.getBottom());

    pBR.addTriangle (outerBevelBounds.getRight(),
                     innerBevelBounds.getBottom(),
                     outerBevelBounds.getRight(),
                     outerBevelBounds.getBottom(),
                     innerBevelBounds.getRight(),
                     innerBevelBounds.getBottom());

    g.saveState();

    g.setColour (baseColour);
    g.strokePath (pL, juce::PathStrokeType (bevelThickness));
    g.strokePath (pR, juce::PathStrokeType (bevelThickness));

    g.setColour (baseColour.darker (0.5f));
    g.strokePath (pT,
                  juce::PathStrokeType (bevelThickness,
                                        juce::PathStrokeType::mitered,
                                        juce::PathStrokeType::square));

    g.setColour (baseColour.brighter (0.5f));
    g.strokePath (pB,
                  juce::PathStrokeType (bevelThickness,
                                        juce::PathStrokeType::mitered,
                                        juce::PathStrokeType::square));

    g.setColour (baseColour);
    g.fillPath (pTL);
    g.fillPath (pTR);
    g.fillPath (pBL);
    g.fillPath (pBR);

    g.restoreState();
}


//==============================================================================


/*
        Draws a line representing the normalised set of samples to the given Image.
        Note the samples must be in the range of 1-0 and the line will be stretched to fit the whole image.
    */
static inline void drawSamplesToImage (const juce::Image& image,
                                       const float*       samples,
                                       int                numSamples,
                                       juce::Colour       colour,
                                       float              thickness)
{
    if (image.isNull()) return;

    jassert (image.getWidth() > 0 && image.getHeight() > 0);

    juce::Graphics g (image);
    g.setColour (colour);
    const auto imageXScale = float (image.getWidth()) / float (numSamples);

    juce::Path p;
    bool       isFirst = true;

    for (int i = 0; i < numSamples; ++i)
    {
        const auto x = i * imageXScale;
        const auto y = image.getHeight() - (samples[i] * image.getHeight());

        if (isFirst)
        {
            p.startNewSubPath (x, y);
            isFirst = false;
        }

        p.lineTo (x, y);
    }

    g.strokePath (p, juce::PathStrokeType (thickness));
}


}  // namespace bav::gui
