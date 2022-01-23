#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

struct RangeTests final : public CoreTest
{
	explicit RangeTests()
	    : CoreTest ("Ranges tests")
	{
	}

private:
	void runTest() final
	{
		// create
		// createBool
		// to value tree
		// from value tree
	}
};

static RangeTests rangeTest;

}  // namespace lemons::tests
