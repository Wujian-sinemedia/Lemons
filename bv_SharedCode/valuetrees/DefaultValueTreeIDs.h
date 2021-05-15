
namespace bav::DefaultValueTreeIds
{

#define BV_DECLARE_VALUETREEID(name) static inline juce::Identifier name { "name" }

BV_DECLARE_VALUETREEID (ParameterGroupNode);
BV_DECLARE_VALUETREEID (Parameter_Float);
BV_DECLARE_VALUETREEID (Parameter_Int);
BV_DECLARE_VALUETREEID (Parameter_Bool);
BV_DECLARE_VALUETREEID (Parameter_Meter);
BV_DECLARE_VALUETREEID (ParameterName);
BV_DECLARE_VALUETREEID (ParameterValue);
BV_DECLARE_VALUETREEID (ParameterDefaultValue);
BV_DECLARE_VALUETREEID (ParameterIsChanging);

BV_DECLARE_VALUETREEID (NonParameterGroupNode);
BV_DECLARE_VALUETREEID (NonParameterNode_Int);
BV_DECLARE_VALUETREEID (NonParameterNode_Bool);
BV_DECLARE_VALUETREEID (NonParameterNode_Float);
BV_DECLARE_VALUETREEID (NonParameterNode_String);
BV_DECLARE_VALUETREEID (NonParameterName);
BV_DECLARE_VALUETREEID (NonParameterValue);
BV_DECLARE_VALUETREEID (NonParameterDefaultValue);

#undef BV_DECLARE_VALUETREEID

}  // namespace
