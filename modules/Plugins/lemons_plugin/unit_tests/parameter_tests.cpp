namespace lemons::tests
{

ParameterTestBase::ParameterTestBase (plugin::Parameter& parameterToUse, const String& testName)
: Test (testName, "PluginParameters"), parameter (parameterToUse)
{
    
}

void ParameterTestBase::runTest()
{
    beginTest ("Parameter traits");
    
    const auto& traits = parameter.getParameterTraits();
    
    createTraitsFromParameter (traits);
    createParameterFromTraits (traits);
}

void ParameterTestBase::createTraitsFromParameter (const plugin::ParameterTraits& traits)
{
    const auto subtest = beginSubtest ("Create traits from parameter");
    
    expect (traits.isValid());
    
    expectEquals (traits.name, parameter.getName (50));
    
    expectEquals (traits.label, parameter.getLabel());
    
    {
        const auto& range = parameter.getNormalisableRange();
        
        expectEquals (traits.range.start, range.start);
        expectEquals (traits.range.end, range.end);
        expectEquals (traits.range.interval, range.interval);
        expectEquals (traits.range.skew, range.skew);
        //        expectEquals (traits.range.symmetricSkew, range.symmetricSkew);
    }
    
    expectEquals (traits.defaultValue, parameter.getNormalizedDefault());
    
    //    expectEquals (traits.isAutomatable, parameter.isAutomatable());
    
    //    expectEquals (traits.isMetaParameter, parameter.isMetaParameter());
    
    expectEquals (traits.category, parameter.getCategory());
}

void ParameterTestBase::createParameterFromTraits (const plugin::ParameterTraits& traits)
{
    const auto subtest = beginSubtest ("Create parameter from traits");
    
    plugin::ParameterList dummyList;
    
    const auto parameter = traits.createParameter (dummyList);
    
    expectEquals (parameter->getName (50), traits.name);
    
    expectEquals (parameter->getLabel(), traits.label);
    
    {
        const auto& range = parameter->getNormalisableRange();
        
        expectEquals (range.start, traits.range.start);
        expectEquals (range.end, traits.range.end);
        expectEquals (range.interval, traits.range.interval);
        expectEquals (range.skew, traits.range.skew);
        //        expectEquals (range.symmetricSkew, traits.range.symmetricSkew);
    }
    
    expectEquals (parameter->getNormalizedDefault(), traits.defaultValue);
    
    //    expectEquals (parameter->isAutomatable(), traits.isAutomatable);
    
    //    expectEquals (parameter->isMetaParameter(), traits.isMetaParameter);
    
    expectEquals (parameter->getCategory(), traits.category);
}

}

