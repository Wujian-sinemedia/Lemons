
/*
    An object that emulates the features & API of a bav::Parameter object, and can be constructed from one, but does not require a juce::RangedAudioParameter object to function.
    This is useful for holding parameters on the GUI side of things -- you can define the specifics for your parameters only once when you construct your bav::Parameter objects, and then in the GUI simply construct a bunch of these objects from the Parameter objects.
 */

namespace bav
{
class FreestandingParameter
{
public:
    FreestandingParameter (const Parameter* const param)
        : parameterNameShort (param->parameterNameShort), parameterNameVerbose (param->parameterNameVerbose), parameterNameVerboseNoSpaces (param->parameterNameVerboseNoSpaces), range (param->orig()->getNormalisableRange()), keyID (param->key())
    {
        currentValue.store (param->getNormalizedDefault());
        currentDefault.store (param->getNormalizedDefault());
        changing.store (false);
        lastActionedValue.store (param->getNormalizedDefault());
    }


    FreestandingParameter (const FloatParameter* const floatParam)
        : FreestandingParameter (static_cast< const Parameter* const > (floatParam))
    {
        floatToStringFunction = floatParam->floatToString;
        stringToFloatFunction = floatParam->stringToFloat;

        valueType = floatValue;
    }


    FreestandingParameter (const IntParameter* const intParam)
        : FreestandingParameter (static_cast< const Parameter* const > (intParam))
    {
        intToString = intParam->intToString;
        stringToInt = intParam->stringToInt;

        floatToStringFunction = [this] (float value, int maxLength)
        {
            return intToString (juce::roundToInt (denormalize (value)), maxLength);
        };

        stringToFloatFunction = [this] (const juce::String& string)
        {
            return static_cast< float > (stringToInt (string));
        };

        valueType = intValue;
    }


    FreestandingParameter (const BoolParameter* const boolParam)
        : FreestandingParameter (static_cast< const Parameter* const > (boolParam))
    {
        boolToString = boolParam->boolToString;
        stringToBool = boolParam->stringToBool;

        floatToStringFunction = [this] (float value, int maxLength)
        {
            return boolToString (value >= 0.5f, maxLength);
        };

        stringToFloatFunction = [this] (const juce::String& string)
        {
            return stringToBool (string) ? 1.0f : 0.0f;
        };

        valueType = boolValue;
    }


    virtual ~FreestandingParameter() = default;


    //==============================================================================


    void setValueNormalized (float value)
    {
        jassert (value >= 0.0f && value <= 1.0f);

        if (currentValue.load() != value)
        {
            currentValue.store (value);
            const auto denorm = denormalize (value);
            listeners.call ([&value, &denorm] (Listener& l)
                            { l.parameterValueChanged (value, denorm); });

            if (onParameterChange)
                bav::callOnMessageThread< float > (onParameterChange,
                                                   getCurrentDenormalizedValue());
        }
    }

    void setValueDenormalized (float value)
    {
        setValueNormalized (normalize (value));
    }

    void setValueDenormalized (int value)
    {
        setValueNormalized (normalize (static_cast< float > (value)));
    }

    void setValueDenormalized (bool value)
    {
        const auto def = value ? 1.0f : 0.0f;
        setValueNormalized (def);
    }

    //==============================================================================

    float getCurrentNormalizedValue() const { return currentValue.load(); }
    float getCurrentDenormalizedValue() const
    {
        return normalize (currentValue.load());
    }

    // these functions return the current denormalized value as the desired type literal
    float getFloatValue() const { return getCurrentDenormalizedValue(); }
    int   getIntValue() const
    {
        return juce::roundToInt (getCurrentDenormalizedValue());
    }
    bool getBoolValue() const { return getCurrentNormalizedValue() >= 0.5f; }

    //==============================================================================

    void beginChangeGesture()
    {
        if (! changing.load())
        {
            changing.store (true);
            listeners.call ([] (Listener& l)
                            { l.parameterGestureChanged (true); });

            if (onGestureStateChange) onGestureStateChange (true);
        }
    }

    void endChangeGesture()
    {
        if (changing.load())
        {
            changing.store (false);
            listeners.call ([] (Listener& l)
                            { l.parameterGestureChanged (false); });

            if (onGestureStateChange) onGestureStateChange (false);
        }
    }

    bool isChanging() const { return changing.load(); }

    //==============================================================================

    void setNormalizedDefault (float value)
    {
        jassert (value >= 0.0f && value <= 1.0f);

        if (value != currentDefault.load())
        {
            currentDefault.store (value);
            const auto denorm = denormalize (value);
            listeners.call ([&value, &denorm] (Listener& l)
                            { l.parameterDefaultValueChanged (value, denorm); });

            if (onDefaultChange)
                bav::callOnMessageThread< float > (onDefaultChange, value);
        }
    }

    void setDefault (float value) { setNormalizedDefault (normalize (value)); }

    void setDefault (int value)
    {
        setNormalizedDefault (normalize (static_cast< float > (value)));
    }

    void setDefault (bool value)
    {
        const auto def = value ? 1.0f : 0.0f;
        setNormalizedDefault (def);
    }

    void refreshDefault() { setNormalizedDefault (getCurrentNormalizedValue()); }

    void resetToDefault() { setValueNormalized (getNormalizedDefault()); }

    float getNormalizedDefault() const { return currentDefault.load(); }

    float getDenormalizedDefault() const
    {
        return denormalize (currentDefault.load());
    }

    //==============================================================================

    float normalize (const float input) const { return range.convertTo0to1 (input); }

    float denormalize (const float input) const
    {
        return range.convertFrom0to1 (input);
    }

    int key() const noexcept { return keyID; }

    //==============================================================================

    void doAction()
    {
        const auto value = getCurrentNormalizedValue();

        if (value != lastActionedValue.load())
        {
            lastActionedValue.store (value);

            if (floatAction)
                floatAction (getCurrentDenormalizedValue());
            else if (intAction)
                intAction (juce::roundToInt (getCurrentDenormalizedValue()));
            else if (boolAction)
                boolAction (getCurrentNormalizedValue() >= 0.5f);
            else if (voidAction)
                voidAction();
        }
    }

    void setFloatAction (std::function< void (float) > action)
    {
        floatAction = std::move (action);
        intAction   = nullptr;
        boolAction  = nullptr;
        voidAction  = nullptr;
    }

    void setIntAction (std::function< void (int) > action)
    {
        intAction   = std::move (action);
        floatAction = nullptr;
        boolAction  = nullptr;
        voidAction  = nullptr;
    }

    void setBoolAction (std::function< void (bool) > action)
    {
        boolAction  = std::move (action);
        floatAction = nullptr;
        intAction   = nullptr;
        voidAction  = nullptr;
    }

    void setVoidAction (std::function< void() > action)
    {
        voidAction  = std::move (action);
        floatAction = nullptr;
        intAction   = nullptr;
        boolAction  = nullptr;
    }

    //==============================================================================

    std::function< void (float) >
                                  onParameterChange;  // this gets the denormalized new float value
    std::function< void (float) > onDefaultChange;
    std::function< void (bool) >  onGestureStateChange;

    //==============================================================================

    const juce::String parameterNameShort;
    const juce::String parameterNameVerbose;
    const juce::String parameterNameVerboseNoSpaces;

    //==============================================================================

    struct Listener
    {
        virtual void parameterValueChanged (float newNormalizedValue,
                                            float newDenormalizedValue)
        {
            juce::ignoreUnused (newNormalizedValue, newDenormalizedValue);
        }

        virtual void parameterDefaultValueChanged (float newNormalizedDefault,
                                                   float newDenormalizedDefault)
        {
            juce::ignoreUnused (newNormalizedDefault, newDenormalizedDefault);
        }

        virtual void parameterGestureChanged (bool gestureIsStarting)
        {
            juce::ignoreUnused (gestureIsStarting);
        }
    };

    //==============================================================================

    void addListener (Listener* l) { listeners.add (l); }
    void removeListener (Listener* l) { listeners.remove (l); }

    //==============================================================================

    juce::String stringFromNormalizedValue (float value, int maxLength)
    {
        jassert (value >= 0.0f && value <= 1.0f);
        return floatToStringFunction (value, maxLength);
    }

    float normalizedValueFromString (const juce::String& string)
    {
        return normalize (stringToFloatFunction (string));
    }

    //==============================================================================

    juce::ValueTree toValueTree() const
    {
        using namespace DefaultValueTreeIds;

        switch (valueType)
        {
            case (floatValue) :
            {
                juce::ValueTree tree {Parameter_Float};

                tree.setProperty (ParameterName, parameterNameVerbose, nullptr);
                tree.setProperty (
                    ParameterValue, getCurrentDenormalizedValue(), nullptr);
                tree.setProperty (
                    ParameterDefaultValue, getDenormalizedDefault(), nullptr);
                tree.setProperty (ParameterIsChanging, isChanging(), nullptr);

                return tree;
            }
            case (intValue) :
            {
                juce::ValueTree tree {Parameter_Int};

                tree.setProperty (ParameterName, parameterNameVerbose, nullptr);
                tree.setProperty (ParameterValue, getIntValue(), nullptr);
                tree.setProperty (ParameterDefaultValue,
                                  juce::roundToInt (getDenormalizedDefault()),
                                  nullptr);
                tree.setProperty (ParameterIsChanging, isChanging(), nullptr);

                return tree;
            }
            case (boolValue) :
            {
                juce::ValueTree tree {Parameter_Bool};

                tree.setProperty (ParameterName, parameterNameVerbose, nullptr);
                tree.setProperty (ParameterValue, getBoolValue(), nullptr);
                tree.setProperty (
                    ParameterDefaultValue, getNormalizedDefault() >= 0.5f, nullptr);
                tree.setProperty (ParameterIsChanging, isChanging(), nullptr);

                return tree;
            }
            default : jassertfalse;
        }
    }


protected:
    std::atomic< float > currentValue;
    std::atomic< float > currentDefault;

    std::atomic< bool > changing;

    juce::NormalisableRange< float > range;

    //

    std::function< juce::String (float, int) >   floatToStringFunction;
    std::function< float (const juce::String&) > stringToFloatFunction;

private:
    const int keyID;

    //==============================================================================

    std::function< void (float) > floatAction;
    std::function< void (int) >   intAction;
    std::function< void (bool) >  boolAction;
    std::function< void() >       voidAction;

    std::atomic< float > lastActionedValue;

    //==============================================================================

    std::function< juce::String (bool, int) >        boolToString;
    std::function< bool (const juce::String& text) > stringToBool;

    std::function< juce::String (int, int) >   intToString;
    std::function< int (const juce::String&) > stringToInt;

    //==============================================================================

    juce::ListenerList< Listener > listeners;

    //==============================================================================

    enum ValueType
    {
        floatValue,
        intValue,
        boolValue
    };

    ValueType valueType;
};


//==============================================================================
//==============================================================================


static inline void convertPluginParametersToFreestanding (
    std::vector< bav::Parameter* >&                 pluginParams,
    juce::OwnedArray< bav::FreestandingParameter >& freestanders)
{
    std::for_each (
        pluginParams.begin(),
        pluginParams.end(),
        [&freestanders] (bav::Parameter* p)
        {
            if (auto* floatParam = dynamic_cast< bav::FloatParameter* > (p))
            {
                freestanders.add (new bav::FreestandingParameter (floatParam));
            }
            else if (auto* intParam = dynamic_cast< bav::IntParameter* > (p))
            {
                freestanders.add (new bav::FreestandingParameter (intParam));
            }
            else if (auto* boolParam = dynamic_cast< bav::BoolParameter* > (p))
            {
                freestanders.add (new bav::FreestandingParameter (boolParam));
            }
            else
            {
                jassertfalse;
            }
        });
}


}  // namespace bav
