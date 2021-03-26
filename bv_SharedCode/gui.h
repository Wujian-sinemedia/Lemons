
/*
    This fie contains some useful GUI helper functions.
*/


namespace bav::gui
{
    
    
    static inline juce::Button::ButtonState buttonStateFromBool (const bool isOn) noexcept
    {
        if (isOn)
            return juce::Button::ButtonState::buttonDown;
        
        return juce::Button::ButtonState::buttonNormal;
    }
    
    
    /*
        Creates a base colour for a component based on the current keyboard and mouse interactivity.
     */
    static inline juce::Colour createBaseColour (const juce::Colour& colour,
                                                 const bool hasKeyboardFocus,
                                                 const bool isMouseOver,
                                                 const bool isButtonDown) noexcept
    {
        const float saturation = hasKeyboardFocus ? 1.3f : 0.9f;
        const juce::Colour baseColour (colour.withMultipliedSaturation (saturation));
        
        if (isButtonDown)
            return baseColour.contrasting (0.2f);
        else if (isMouseOver)
            return baseColour.contrasting (0.1f);
        
        return baseColour;
    }
    
    
    /*
        Draws a square bevel around a given rectange.
        This is useful for insetting components and giving them a border.
     */
    static void drawBevel (juce::Graphics& g, juce::Rectangle<float> innerBevelBounds,
                           float bevelThickness, const juce::Colour& baseColour)
    {
        juce::Rectangle<float> outerBevelBounds (innerBevelBounds.expanded (bevelThickness, bevelThickness));
        juce::Rectangle<float> centreBevelBounds (innerBevelBounds.expanded (bevelThickness * 0.5f, bevelThickness * 0.5f));
        
        juce::Path pL, pR, pT, pB, pTL, pTR, pBL, pBR;
        pL.startNewSubPath (centreBevelBounds.getTopLeft());
        pL.lineTo (centreBevelBounds.getBottomLeft());
        
        pR.startNewSubPath (centreBevelBounds.getTopRight());
        pR.lineTo (centreBevelBounds.getBottomRight());
        
        pT.startNewSubPath (centreBevelBounds.getTopLeft());
        pT.lineTo (centreBevelBounds.getTopRight());
        
        pB.startNewSubPath (centreBevelBounds.getBottomLeft());
        pB.lineTo (centreBevelBounds.getBottomRight());
        
        pTL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getY(),
                         outerBevelBounds.getX(), innerBevelBounds.getY(),
                         innerBevelBounds.getX(), innerBevelBounds.getY());
        
        pTR.addTriangle (outerBevelBounds.getRight(), outerBevelBounds.getY(),
                         outerBevelBounds.getRight(), innerBevelBounds.getY(),
                         innerBevelBounds.getRight(), innerBevelBounds.getY());
        
        pBL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getBottom(),
                         outerBevelBounds.getX(), innerBevelBounds.getBottom(),
                         innerBevelBounds.getX(), innerBevelBounds.getBottom());
        
        pBR.addTriangle (outerBevelBounds.getRight(), innerBevelBounds.getBottom(),
                         outerBevelBounds.getRight(), outerBevelBounds.getBottom(),
                         innerBevelBounds.getRight(), innerBevelBounds.getBottom());
        
        g.saveState();
        
        g.setColour (baseColour);
        g.strokePath (pL, juce::PathStrokeType (bevelThickness));
        g.strokePath (pR, juce::PathStrokeType (bevelThickness));
        
        g.setColour (baseColour.darker (0.5f));
        g.strokePath (pT, juce::PathStrokeType (bevelThickness, juce::PathStrokeType::mitered, juce::PathStrokeType::square));
        
        g.setColour (baseColour.brighter (0.5f));
        g.strokePath (pB, juce::PathStrokeType (bevelThickness, juce::PathStrokeType::mitered, juce::PathStrokeType::square));
        
        g.setColour (baseColour);
        g.fillPath (pTL);
        g.fillPath (pTR);
        g.fillPath (pBL);
        g.fillPath (pBR);
        
        g.restoreState();
    }
    
    
    
    /**
     Helper function to serialise a system font to a file.
     This is useful if you want to include a custom font in an application so that is
     guarenteed to be avialiable cross-platform.
     
     Once you have the generated file you can include it as BinarayData (as generated
     by theIntorjucer) and then reload it in a look and feel.
     
     In your LookAndFeel subclass overide getTypeFaceForFont and create a Typeface::Ptr
     to hold the font e.g.
     
     @code
     //==============================================================================
     class FontLookAndFeel : public LookAndFeel
     {
     public:
        const Typeface::Ptr getTypefaceForFont (const Font& font);
     
     private:
        Typeface::Ptr customTypeface;
     }
     @endcode
     
     Then in your constructor reload the font and the return your custom typeface
     in getTypefaceForFont if the requested font names match.
     
     @code
     //==============================================================================
     FontLookAndFeel::FontLookAndFeel()
     {
        ScopedPointer<MemoryInputStream> fontStream (new MemoryInputStream (BinaryData::Custom_Font,
                                                                            BinaryData::Custom_FontSize,
                                                                            false));
        if (fontStream != nullptr)
            customTypeface = new CustomTypeface (*fontStream);
     }
     //==============================================================================
     const Typeface::Ptr FontLookAndFeel::getTypefaceForFont (const Font& font)
     {
        if (customTypeface != nullptr && font.getTypefaceName() == customTypeface->getName())
            return customTypeface;
     
        return LookAndFeel::getTypefaceForFont (font);
     }
     @endcode
     */
    static bool serializeFont (const juce::Font& font, juce::File& destinationFile, int maxNumChars = 127)
    {
        destinationFile.deleteFile();
        juce::ScopedPointer<juce::FileOutputStream> outFileStream (destinationFile.createOutputStream());
        
        juce::CustomTypeface customTypeface;
        customTypeface.setCharacteristics (font.getTypefaceName(), font.getAscent(),
                                           font.isBold(), font.isItalic(), ' ');
        customTypeface.addGlyphsFromOtherTypeface (*font.getTypeface(), 0, maxNumChars);
        
        return customTypeface.writeToStream (*outFileStream);
    }
    
    
    
    /*
        List of icons to be used with the createIcon method.
     */
    enum IconType
    {
        Stop,
        Play,
        Cue,
        Pause,
        Next,
        Previous,
        ShuffleForward,
        ShuffleBack,
        Eject,
        Cross,
        Add,
        Search,
        Power,
        Bypass,
        GoUp,
        Infinity,
        DownTriangle,
        Info,
        Loop,
        Slow,
        Speaker,
        MutedSpeaker
    };
    
    /*
        Creates an icon in a given colour.
     */
    static inline juce::DrawablePath createIcon (IconType icon, juce::Colour colour)
    {
        switch (icon)
        {
            case Stop:
            {
                juce::Path squarePath;
                squarePath.addRectangle (100.0f, 100.0f, 100.0f, 100.0f);
                
                juce::DrawablePath squareImage;
                squareImage.setFill (colour);
                squareImage.setPath (squarePath);
                
                return squareImage;
            }
            case Play:
            {
                juce::Path trianglePath;
                trianglePath.addTriangle (0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 50.0f);
                
                juce::DrawablePath triangleImage;
                triangleImage.setFill (colour);
                triangleImage.setPath (trianglePath);
                
                return triangleImage;
            }
            case Pause:
            {
                juce::Path pausePath;
                pausePath.addRectangle (0.0f, 0.0f, 20.0f, 100.0f);
                pausePath.addRectangle (60.0f, 0.0f, 20.0f, 100.0f);
                
                juce::DrawablePath pauseImage;
                pauseImage.setFill (colour);
                pauseImage.setPath (pausePath);
                
                return pauseImage;
            }
            case Cue:
            {
                juce::Path p;
                p.addRectangle (0, 0, 30, 50);
                p.addArrow (juce::Line<float> (0.0f, 50.0f, 100.0f, 50.0f),
                            30.0f, 100.0f, 40.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (colour);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Next:
            {
                juce::Path p;
                p.addTriangle (0.0f, 0.0f, 0.0f, 100.0f, 90.0f, 50.0f);
                p.addRectangle (90, 0, 10, 100);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (colour);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Previous:
            {
                juce::Path p;
                p.addTriangle (100.0f, 100.0f, 100.0f, 0.0f, 10.0f, 50.0f);
                p.addRectangle (0, 0, 10, 100);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (colour);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case ShuffleForward:
            {
                juce::Path p;
                p.addTriangle (0.0f, 0.0f, 0.0f, 100.0f, 50.0f, 50.0f);
                p.addTriangle (50.0f, 0.0f, 50.0f, 100.0f, 100.0f, 50.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (colour);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case ShuffleBack:
            {
                juce::Path p;
                p.addTriangle (0.0f, 50.0f, 75.0f, 0.0f, 75.0f, 100.0f);
                p.addTriangle (75.0f, 50.0f, 150.0f, 0.0f, 150.0f, 100.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (colour);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Eject:
            {
                juce::Path p;
                p.addTriangle (0, 65, 100, 65, 50, 0);
                p.addRectangle (0, 80, 100, 20);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (colour);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Cross:
            {
                juce::Path p;
                p.startNewSubPath (0.0f, 0.0f);
                p.lineTo (100.0f, 100.0f);
                p.startNewSubPath (100.0f, 0.0f);
                p.lineTo (0.0f, 100.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
                drawablePath.setStrokeFill (colour);
                drawablePath.setStrokeThickness (15);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Add:
            {
                juce::Path p;
                p.startNewSubPath (50.0f, 0.0f);
                p.lineTo (50.0f, 100.0f);
                p.startNewSubPath (0.0f, 50.0f);
                p.lineTo (100.0f, 50.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
                drawablePath.setStrokeFill (colour);
                drawablePath.setStrokeThickness (15);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Search:
            {
                juce::Path p;
                p.addEllipse (20, 0, 80, 80);
                p.startNewSubPath (0.0f, 100.0f);
                p.lineTo (35.0f, 65.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
                drawablePath.setStrokeFill (colour);
                drawablePath.setStrokeThickness (15);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Power:
            {
                juce::Path p;
                p.addArc (0.0f, 20.0f, 100.0f, 100.0f, 0.18f * math::float_Pi, 2.0f * math::float_Pi - (0.18f * math::float_Pi), true);
                p.startNewSubPath (50.0f, 0.0f);
                p.lineTo (50.0f, 70.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
                drawablePath.setStrokeFill (colour);
                drawablePath.setStrokeThickness (10.0f);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case Bypass:
            {
                juce::Path p;
                p.startNewSubPath (50.0f, 0.0f);
                p.lineTo (50.0f, 30.0f);
                p.lineTo (80.0f, 70.0f);
                p.startNewSubPath (50.0f, 70.0f);
                p.lineTo (50.0f, 100.0f);
                
                juce::DrawablePath drawablePath;
                drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
                drawablePath.setStrokeFill (colour);
                drawablePath.setStrokeThickness (10);
                drawablePath.setPath (p);
                
                return drawablePath;
            }
            case GoUp:
            {
                juce::Path arrowPath;
                arrowPath.addArrow (juce::Line<float> (50.0f, 100.0f, 50.0f, 0.0f),
                                    40.0f, 100.0f, 50.0f);
                
                juce::DrawablePath arrowImage;
                arrowImage.setFill (colour);
                arrowImage.setPath (arrowPath);
                
                return arrowImage;
            }
            case Infinity:
            {
                juce::Path infPath;
                infPath.addEllipse (0.0f, 0.0f, 50.0f, 50.0f);
                infPath.startNewSubPath (50.0f, 0.0f);
                infPath.addEllipse (50.0f, 0.0f, 50.0f, 50.0f);
                
                juce::DrawablePath infImg;
                infImg.setFill (juce::Colours::white.withAlpha (0.0f));
                infImg.setStrokeFill (colour);
                infImg.setStrokeThickness (10.0f);
                infImg.setPath (infPath);
                
                return infImg;
            }
            case DownTriangle:
            {
                juce::Path trianglePath;
                trianglePath.addTriangle (0.0f, 0.0f, 100.0f, 0.0f, 50.0f, 100.0f);
                
                juce::DrawablePath triangleImage;
                triangleImage.setFill (colour);
                triangleImage.setPath (trianglePath);
                
                return triangleImage;
            }
            case Info:
            {
                juce::Path circlePath;
                circlePath.addEllipse (0.0f, 0.0f, 100.0f, 100.0f);
                
                juce::DrawablePath circleImage;
                circleImage.setFill (colour);
                circleImage.setPath (circlePath);
                
                return circleImage;
            }
            case Loop:
            {
                juce::Path loopPath;
                loopPath.addRoundedRectangle (0.0f, 0.0f, 150.0f, 50.0f, 25.0f);
                loopPath.addTriangle (100.0f, -10.0f, 100.0f, 10.0f, 110.0f, 0.0f);
                loopPath.addTriangle (40.0f, 50.0f, 50.0f, 60.0f, 50.0f, 40.0f);
                
                juce::DrawablePath loopImage;
                loopImage.setFill (juce::Colours::white.withAlpha(0.0f));
                loopImage.setStrokeFill (colour);
                loopImage.setStrokeThickness (15.0f);
                loopImage.setPath (loopPath);
                
                return loopImage;
            }
            case Slow:
            {
                juce::Path p;
                p.addPolygon (juce::Point<float> (50.0f, 50.0f),
                              8,
                              50.0f,
                              math::float_Pi * -0.125f);
                
                juce::DrawablePath dp;
                dp.setFill (colour);
                dp.setStrokeFill (colour);
                dp.setStrokeThickness (0.0f);
                dp.setPath (p);
                
                return dp;
            }
            case Speaker:
            {
                juce::Path p;
                p.addRoundedRectangle (0.0f, 33.0f, 33.0f, 33.0f, 2.0f);
                p.addTriangle (7.5f, 50.0f, 55.0f, 6.5f, 55.0f, 93.5f);
                p.addArc (60.0f, 30.0f, 12.0f, 40.0f, math::float_Pi * 0.15f, math::float_Pi * 0.85f, true);
                p.addArc (70.0f, 20.0f, 16.0f, 60.0f, math::float_Pi * 0.15f, math::float_Pi * 0.85f, true);
                p.addArc (80.0f, 10.0f, 20.0f, 80.0f, math::float_Pi * 0.15f, math::float_Pi * 0.85f, true);
                
                juce::DrawablePath dp;
                dp.setFill (colour);
                dp.setStrokeFill (colour);
                dp.setStrokeThickness (5.0f);
                dp.setPath (p);
                
                return dp;
            }
            case MutedSpeaker:
            {
                juce::Path p;
                p.addRoundedRectangle (0.0f, 33.0f, 33.0f, 33.0f, 2.0f);
                p.addTriangle (7.5f, 50.0f, 55.0f, 6.5f, 55.0f, 93.5f);
                p.scaleToFit (0.0f, 0.0f, 100.0f, 100.0f, true);
                
                juce::DrawablePath dp;
                dp.setFill (colour);
                dp.setStrokeFill (colour);
                dp.setStrokeThickness (5.0f);
                dp.setPath (p);
                
                return dp;
            }

            default:
            {
                juce::DrawablePath blank;
                return blank;
            }
        }
    }
    
    
}  // namespace
