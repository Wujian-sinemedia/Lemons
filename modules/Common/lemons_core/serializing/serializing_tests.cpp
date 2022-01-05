#if ! LEMONS_UNIT_TESTS
#error
#endif


namespace lemons::tests
{

struct CoreSerializingTests final : public CoreTest
{
    explicit CoreSerializingTests()
    : CoreTest ("Core serializing functions")
    { }
    
private:
    void runTest() final
    {
        beginTest ("Memory block to/from string");
        
        beginTest ("ValueTree to/from JSON");
    }
};

//static CoreSerializingTests coreSerializingTests;

}
