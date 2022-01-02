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

struct MetaParameterBase
{
public:

	explicit MetaParameterBase (const ParameterList& parameterListToUse);

	virtual ~MetaParameterBase() = default;

	void applyAllConnections() const;

	[[nodiscard]] bool hasConnection (const Parameter& parameter) const;

	bool removeConnection (const Parameter& parameter);

	void createOrEditConnection (Parameter& parameter, float minAmt, float maxAmt);

	[[nodiscard]] ValueTree saveConnectionsToValueTree() const;

	void loadConnectionsFromValueTree (const ValueTree& tree);

	[[nodiscard]] virtual Parameter& getParameter() = 0;

	[[nodiscard]] virtual const Parameter& getParameter() const = 0;

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


template <typename ValueType>
class MetaParameter : public TypedParameter<ValueType>
    , public MetaParameterBase
{
public:
	explicit MetaParameter (
	    const ParameterList& parameterListToUse,
	    ValueType minimum, ValueType maximum, ValueType defaultValue, const String& paramName,
	    ValToStringFunc<ValueType> stringFromValue = nullptr,
	    StringToValFunc<ValueType> valueFromString = nullptr,
	    const String& paramLabel = {}, bool isAutomatable = true,
	    ParameterCategory parameterCategory = ParameterCategory::genericParameter);

	explicit MetaParameter (const ParameterTraits& traits, const ParameterList& parameterListToUse);

	[[nodiscard]] ValueTree saveToValueTree() const override;

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
