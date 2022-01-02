#pragma once

namespace lemons::tests
{

struct ParameterTestBase : public Test
{
public:
    explicit ParameterTestBase (plugin::Parameter& parameterToUse, const String& testName);
    
protected:
    void runTest() override;
    
private:
    void createTraitsFromParameter (const plugin::ParameterTraits& traits);
    
    void createParameterFromTraits (const plugin::ParameterTraits& traits);
    
    plugin::Parameter& parameter;
};

}
