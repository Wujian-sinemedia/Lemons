/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#pragma once

namespace lemons::plugin
{

/** @defgroup meta_parameters Meta parameters
    @ingroup parameters
 */


/** @ingroup meta_parameters
    A type-erased interface for a parameter that can be connected to other parameters and control their values.
    @see MetaParameter
 */
struct MetaParameterBase
{
public:

	/** Creates a meta parameter interface.
	    You must give this class a ParameterList to reference, because when loading a state, it will search the list for parameter IDs that match each saved connections' state */
	explicit MetaParameterBase (const ParameterList& parameterListToUse);

	/** Destructor. */
	virtual ~MetaParameterBase() = default;

	/** Applies the meta parameter's current value to all of its connections. */
	void applyAllConnections() const;

	/** Returns true if this meta parameter is currently connected to the given parameter. */
	[[nodiscard]] bool hasConnection (const Parameter& parameter) const;

	/** Removes the connection to the given parameter, if any.
	    @returns True if a connection previously existed to the given parameter
	 */
	bool removeConnection (const Parameter& parameter);

	/** Creates or edits a connection to a given parameter.
	    If a connection to the given parameter already exists, it will be remapped to the new min & max amounts; otherwise, a new connection will be created.
	    The min and max values should be between 0.f and 1.f, and can be thought of as the percent effect this meta parameter will have on this parameter.
	 */
	void createOrEditConnection (Parameter& parameter, float minAmt, float maxAmt);

	/** Saves the state of this meta parameter's connections to a ValueTree.
	    @see loadConnectionsFromValueTree()
	 */
	[[nodiscard]] ValueTree saveConnectionsToValueTree() const;

	/** Recreates the state of a saved set of connections.
	    @see saveConnectionsToValueTree()
	 */
	void loadConnectionsFromValueTree (const ValueTree& tree);

	/** Returns the actual parameter object that is controlling this meta parameter -- ie, the parameter that is sending out its values to control the others. */
	[[nodiscard]] virtual Parameter& getParameter() = 0;

	/** Returns the actual parameter object that is controlling this meta parameter -- ie, the parameter that is sending out its values to control the others. */
	[[nodiscard]] virtual const Parameter& getParameter() const = 0;

	/** @internal */
	static constexpr auto valueTreeType = "Connections";

private:
	struct Connection final
	{
		explicit Connection (const ParameterList& list);

		[[nodiscard]] ValueTree saveToValueTree() const;

		void loadFromValueTree (const ValueTree& tree);

		void apply (float newMetaVal) const;

		[[nodiscard]] bool isMapped() const noexcept;

		[[nodiscard]] bool isMappedTo (const Parameter& param) const;

		void editMapping (Parameter& param, float minimum, float maximum);

		void removeMapping();

		static constexpr auto valueTreeType = "Connection";

	private:
		const ParameterList& paramList;

		Parameter* parameter { nullptr };

		float min { 0.f }, max { 1.f };
	};

	[[nodiscard]] Connection& getConnectionToAssign (const Parameter& parameter);

	[[nodiscard]] Connection*       getConnection (const Parameter& parameter);
	[[nodiscard]] const Connection* getConnection (const Parameter& parameter) const;

	[[nodiscard]] Connection* getInvalidConnection();

	juce::OwnedArray<Connection> connections;

	const ParameterList& parameterList;
};


/** @ingroup meta_parameters
    A meta parameter class that implements the MetaParameterBase interface.
    @see MetaParameterBase, TypedParameter
 */
template <typename ValueType>
class MetaParameter : public TypedParameter<ValueType>
    , public MetaParameterBase
{
public:
	/** Creates a meta parameter.
	    @param parameterListToUse The parameter list for this meta parameter to use to rebuild its connections when loading state.
	    @param minimum The minimum value the parameter can have.
	    @param maximum The maximum value the parameter can have.
	    @param defaultValue The initial default value for the parameter.
	    @param paramName The name of this parameter.
	    @param stringFromValue An optional lambda function that converts this parameter's value to a text description.
	    @param valueFromString An optional lambda function that converts a text input string to a value for this parameter.
	    @param paramLabel An optional label to use for this parameter's units.
	    @param isAutomatable Boolean flag that indicates whether this parameter should be automatable in the user's DAW.
	    @param parameterCategory An optional parameter category. See juce::AudioProcessorParameter::Category.
	 */
	explicit MetaParameter (
	    const ParameterList& parameterListToUse,
	    ValueType minimum, ValueType maximum, ValueType defaultValue, const String& paramName,
	    ValToStringFunc<ValueType> stringFromValue = nullptr,
	    StringToValFunc<ValueType> valueFromString = nullptr,
	    const String& paramLabel = {}, bool isAutomatable = true,
	    ParameterCategory parameterCategory = ParameterCategory::genericParameter);

	/** Creates a meta parameter from a ParameterTraits object. */
	explicit MetaParameter (const ParameterTraits& traits, const ParameterList& parameterListToUse);

	/** Saves the state of this meta parameter to a ValueTree. */
	[[nodiscard]] ValueTree saveToValueTree() const override;

	/** Loads the state of this meta parameter from a ValueTree. */
	void loadFromValueTree (const ValueTree& tree) override;

private:

	[[nodiscard]] Parameter& getParameter() final;

	[[nodiscard]] const Parameter& getParameter() const final;


	struct Updater : Parameter::Listener
	{
		Updater (MetaParameter& paramToUse);

	private:
		void parameterValueChanged (float) final;

		MetaParameter& metaParam;
	};

	Updater u { *this };
};

}  // namespace lemons::plugin
