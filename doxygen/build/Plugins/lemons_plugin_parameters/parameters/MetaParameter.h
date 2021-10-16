
/** @ingroup parameters
 *  @{
 */
#pragma once

namespace lemons::plugin
{

template < typename ValueType >
class MetaParameter : public TypedParameter< ValueType >
{
public:
    MetaParameter (ParameterList& parameterListToUse,
                   ValueType minimum, ValueType maximum, ValueType defaultValue, String paramName,
                   std::function< String (ValueType, int) >   stringFromValue = nullptr,
                   std::function< ValueType (const String&) > valueFromString = nullptr,
                   String paramLabel = {}, bool isAutomatable = true,
                   juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter);

    void removeInvalidConnections();

    bool hasConnection (Parameter& parameter) const;

    void removeConnection (Parameter& parameter);

    void createOrEditConnection (Parameter& parameter, float minAmt, float maxAmt);

    template < typename Type >
    void createOrEditConnection (TypedParameter< Type >& parameter, Type minAmt, Type maxAmt)
    {
        createOrEditConnection (*static_cast< Parameter* > (&parameter),
                                parameter.normalize (static_cast< float > (minAmt)),
                                parameter.normalize (static_cast< float > (maxAmt)));
    }

    void createOrEditConnection (BoolParameter& parameter);

private:
    void serialize (TreeReflector& ref) final;

    /*---------------------------------------------------------*/

    struct Connection : SerializableData
    {
        Connection() = default;

        Parameter* parameter {nullptr};
        float      min {0.f}, max {1.f};

    private:
        friend class MetaParameter;

        void serialize (TreeReflector& ref) final;

        void apply (float newMetaVal);

        ParameterList* paramList {nullptr};
    };

    /*---------------------------------------------------------*/

    Connection*       getConnection (Parameter& parameter);
    const Connection* getConnection (Parameter& parameter) const;

    Connection* getInvalidConnection();

    /*---------------------------------------------------------*/

    struct Updater : Parameter::Listener
    {
        Updater (MetaParameter& paramToUse);

    private:
        void parameterValueChanged (float newNormalizedValue) final;

        MetaParameter& metaParam;
    };

    /*---------------------------------------------------------*/

    friend struct Updater;

    Updater u {*this};

    juce::Array< Connection > connections;

    ParameterList& parameterList;
};


struct DefaultMetaParameter : MetaParameter< int >
{
    DefaultMetaParameter (ParameterList& parameterListToUse, String paramName);
};

}  // namespace lemons::plugin

namespace lemons
{

using MetaParam = plugin::ParameterHolder< plugin::DefaultMetaParameter >;

}

/** @}*/
