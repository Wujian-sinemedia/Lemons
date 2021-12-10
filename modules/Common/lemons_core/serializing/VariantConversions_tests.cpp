#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

VariantConversionTests::VariantConversionTests()
: CoreTest ("Variant conversions")
{ }

void VariantConversionTests::runTest()
{
    beginTest ("Variant conversions - JUCE classes");
    
    {
        const auto subtest = beginSubtest ("ADSR::Parameters");
    }
    
    {
        const auto bufferSubtest = beginSubtest ("Audio buffers");
        
        {
            const auto subtest = beginSubtest ("float");
        }
        {
            const auto subtest = beginSubtest ("double");
        }
    }
    
    {
        const auto subtest = beginSubtest ("BigInteger");
    }
    
    {
        const auto subtest = beginSubtest ("Colour");
    }
    
    {
        const auto subtest = beginSubtest ("Identifier");
    }
    
    {
        const auto subtest = beginSubtest ("Image");
    }
    
    {
        const auto subtest = beginSubtest ("IPAddress");
    }
    
    {
        const auto subtest = beginSubtest ("Justification");
    }
    
    {
        const auto subtest = beginSubtest ("MAC Address");
    }
    
    {
        const auto subtest = beginSubtest ("MemoryBlock");
    }
    
    {
        const auto subtest = beginSubtest ("MidiBuffer");
    }
    
    {
        const auto subtest = beginSubtest ("MidiFile");
    }
    
    {
        const auto subtest = beginSubtest ("MidiMessage");
    }
    
    {
        const auto subtest = beginSubtest ("NamedValueSet");
    }
    
    {
        const auto subtest = beginSubtest ("RelativeTime");
    }
    
    {
        const auto subtest = beginSubtest ("StringArray");
    }
    
    {
        const auto subtest = beginSubtest ("StringPairArray");
    }
    
    {
        const auto subtest = beginSubtest ("Time");
    }
    
    {
        const auto subtest = beginSubtest ("UUID");
    }
    
    {
        const auto subtest = beginSubtest ("URL");
    }
    
    {
        const auto subtest = beginSubtest ("ValueTree");
    }
    
    /*---------------------------------------------------------------------------------------------------------------------------------*/

    beginTest ("Variant conversions - Lemons classes");
    
    {
        const auto subtest = beginSubtest ("Dimensions");
    }
    
    {
        const auto subtest = beginSubtest ("Version");
    }
}

}

#endif
