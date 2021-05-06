

namespace bav::ParameterValueConversionLambdas
{

    /* Boolean toggles */
    static const std::function< juce::String (bool value, int maximumStringLength) > toggle_stringFromBool =
    [](bool value, int maxLength)
    {
        return value ? TRANS("On").substring(0, maxLength) : TRANS("Off").substring(0, maxLength);
    };
    
    static const std::function< bool (const juce::String& text) > toggle_boolFromString =
    [](const juce::String& text)
    {
        return (text.containsIgnoreCase (TRANS("On")) || text.containsIgnoreCase (TRANS("Yes")) || text.containsIgnoreCase (TRANS("True")));
    };
    
    /* Float gain */
    static const std::function< juce::String (float value, int maximumStringLength) > gain_stringFromFloat =
    [](float value, int maxLength)
    {
        return (juce::String(value) + " " + TRANS("dB")).substring(0, maxLength);
    };
    
    static const std::function< float (const juce::String& text) > gain_floatFromString =
    [](const juce::String& text)
    {
        const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS("dB"));
        
        if (token_location > -1)
            return text.substring (0, token_location).trim().getFloatValue();
        
        return text.trim().getFloatValue();
    };
    
    /* Integer percent */
    static const std::function< juce::String (int value, int maximumStringLength) > pcnt_stringFromInt =
    [](int value, int maxLength)
    {
        return (juce::String(value) + "%").substring(0, maxLength);
    };
    
    static const std::function< int (const juce::String& text) > pcnt_intFromString =
    [](const juce::String& text)
    {
        const auto token_location = text.indexOf ("%");
        
        if (token_location > -1)
            return text.substring (0, token_location).trim().getIntValue();
        
        return text.trim().getIntValue();
    };
    
    /* Seconds (as float) */
    static const std::function< juce::String (float value, int maximumStringLength) > sec_stringFromFloat =
    [](float value, int maxLength)
    {
        return (juce::String(value) + " " + TRANS("sec")).substring(0, maxLength);
    };
    
    static const std::function< float (const juce::String& text) > sec_floatFromString =
    [](const juce::String& text)
    {
        const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS("sec"));
        
        if (token_location > -1)
            return text.substring (0, token_location).trim().getFloatValue();
        
        return text.trim().getFloatValue();
    };
    
    /* Hz - as float */
    static const std::function< juce::String (float value, int maximumStringLength) > hz_stringFromFloat =
    [](float value, int maxLength)
    {
        auto string = (value < 1000.0f) ? juce::String (value) + " " + TRANS("Hz")
        : juce::String (value * 0.001f) + " " + TRANS("kHz");
        
        return string.substring(0, maxLength);
    };
    
    static const std::function< float (const juce::String& text) > hz_floatFromString =
    [](juce::String text)
    {
        const auto kHz_token_location = text.indexOfWholeWordIgnoreCase (TRANS("kHz"));
        
        if (kHz_token_location > -1)
            return text.substring (0, kHz_token_location).trim().getFloatValue() * 1000.0f;
        
        const auto hz_token_location = text.indexOfWholeWordIgnoreCase (TRANS("Hz"));
        
        if (hz_token_location > -1)
            return text.substring (0, hz_token_location).trim().getFloatValue();
        
        return text.trim().getFloatValue();
    };
    
    /* Semitones (as integer) */
    static const std::function< juce::String (int value, int maximumStringLength) > st_stringFromInt =
    [](int value, int maxLength)
    {
        return (juce::String(value) + " " + TRANS("st")).substring(0, maxLength);
    };
    
    static const std::function< int (const juce::String& text) > st_intFromString =
    [](const juce::String& text)
    {
        const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS("st"));
        
        if (token_location > -1)
            return text.substring (0, token_location).trim().getIntValue();
        
        return text.trim().getIntValue();
    };
    
    /* Midipitch (as integer) */
    static const std::function< juce::String (int value, int maximumStringLength) > pitch_stringFromInt =
    [](int value, int maxLength)
    {
        return bav::midi::pitchToString (value, true).substring(0, maxLength);
    };
    
    static const std::function< int (const juce::String& text) > pitch_intFromString =
    [](const juce::String& text)
    {
        static const auto pitchClassTokens = juce::String("AaBbCcDdEeFfGg#") + bav::gui::getSharpSymbol() + bav::gui::getFlatSymbol();
        
        if (text.containsAnyOf (pitchClassTokens))
            return bav::midi::stringToPitch (text.trim());
        
        return text.trim().getIntValue();
    };
    
    /* Normalized percent as integer */
    static const std::function< juce::String (int value, int maximumStringLength) > normPcnt_stringFromInt =
    [](int value, int maxLength)
    {
        return (juce::String(value * 100.0f) + "%").substring(0, maxLength);
    };
    
    static const std::function< int (const juce::String& text) > normPcnt_intFromString =
    [](const juce::String& text)
    {
        const auto token_location = text.indexOf ("%");
        
        if (token_location > -1)
            return text.substring (0, token_location).trim().getFloatValue() * 0.01f;
        
        return text.trim().getFloatValue();
    };

    /* MIDI pan -> string (eg, 127 = 50R, 64 = C, 0 = 50L) */
    static const std::function< juce::String (int value, int maxLength) > midiPan_stringFromInt =
    [](int value, int maxLength)
    {
        return bav::midiPanIntToString (value).substring(0, maxLength);
    };

    static const std::function< int (const juce::String& text) > midiPan_intFromString =
    [](const juce::String& text)
    {
        return bav::midiPanStringToInt (text);
    };

} // namespace


namespace bav
{

/* Updates the ValueTree with changes from the parameter object */
class ParameterToValueTreeAttachment   :     private juce::Timer,
                                             public  juce::AudioProcessorParameter::Listener
{
public:
    ParameterToValueTreeAttachment (bav::Parameter* paramToUse,
                                    juce::ValueTree& treeToUse,
                                    juce::Identifier paramIdentifierToUse,
                                    juce::Identifier paramGestureIdentifierToUse)
    : param (paramToUse),
    tree (treeToUse),
    paramIdentifier (paramIdentifierToUse),
    paramGestureIdentifier (paramGestureIdentifierToUse)
    {
        param->orig()->addListener (this);
        startTimerHz (10);
        lastSentValue = param->getCurrentDenormalizedValue();
        lastSentChangeState = false;
    }
    
    virtual ~ParameterToValueTreeAttachment() override
    {
        stopTimer();
        param->orig()->removeListener (this);
    }
    
    void timerCallback() override final
    {
        const auto newValue = param->getCurrentDenormalizedValue();
        
        if (lastSentValue != newValue)
        {
            lastSentValue = newValue;
            tree.setProperty (paramIdentifier, newValue, nullptr);
        }
        
        const auto changeState = isChanging.load();
        
        if (lastSentChangeState != changeState)
        {
            lastSentChangeState = changeState;
            tree.setProperty (paramGestureIdentifier, isChanging.load(), nullptr);
        }
    }
    
    void parameterValueChanged (int, float) override final { }
    
    void parameterGestureChanged (int, bool gestureIsStarting) override final
    {
        isChanging.store (gestureIsStarting);
    }
    
    
private:
    bav::Parameter* const param;
    juce::ValueTree& tree;
    
    float lastSentValue;
    bool  lastSentChangeState;
    std::atomic<bool> isChanging;
    
    const juce::Identifier paramIdentifier;
    const juce::Identifier paramGestureIdentifier;
};


/* Updates the parameter object with changes from the ValueTree */
class ValueTreeToParameterAttachment   :    public juce::ValueTree::Listener
{
public:
    ValueTreeToParameterAttachment (bav::Parameter* paramToUse,
                                    juce::ValueTree& treeToUse,
                                    juce::Identifier paramIdentifierToUse,
                                    juce::Identifier paramGestureIdentifierToUse)
    : param (paramToUse),
    tree (treeToUse),
    paramIdentifier (paramIdentifierToUse),
    paramGestureIdentifier (paramGestureIdentifierToUse)
    {
        tree.addListener (this);
        lastSentValue = param->getCurrentDenormalizedValue();
        lastSentChangeState = false;
    }
    
    ~ValueTreeToParameterAttachment() override
    {
        tree.removeListener (this);
    }
    
    void valueTreePropertyChanged (juce::ValueTree& ltree, const juce::Identifier& property) override final
    {
        if (property == paramIdentifier)
        {
            const float newValue = ltree.getProperty (paramIdentifier);
            
            if (lastSentValue != newValue)
            {
                lastSentValue = newValue;
                param->orig()->setValueNotifyingHost (newValue);
            }
        }
        else if (property == paramGestureIdentifier)
        {
            const bool isNowChanging = ltree.getProperty (paramGestureIdentifier);
            
            if (lastSentChangeState != isNowChanging)
            {
                lastSentChangeState = isNowChanging;
                
                if (isNowChanging)
                    param->orig()->beginChangeGesture();
                else
                    param->orig()->endChangeGesture();
            }
        }
    }
    
    
private:
    bav::Parameter* const param;
    juce::ValueTree& tree;
    
    float lastSentValue;
    bool  lastSentChangeState;
    
    const juce::Identifier paramIdentifier;
    const juce::Identifier paramGestureIdentifier;
};


struct ParameterAttachment  :   ParameterToValueTreeAttachment,
                                ValueTreeToParameterAttachment
{
    ParameterAttachment (bav::Parameter* paramToUse,
                         juce::ValueTree& treeToUse,
                         juce::Identifier paramIdentifierToUse,
                         juce::Identifier paramGestureIdentifierToUse)
    : ParameterToValueTreeAttachment (paramToUse, treeToUse, paramIdentifierToUse, paramGestureIdentifierToUse),
      ValueTreeToParameterAttachment (paramToUse, treeToUse, paramIdentifierToUse, paramGestureIdentifierToUse)
    { }
};

}  // namespace
