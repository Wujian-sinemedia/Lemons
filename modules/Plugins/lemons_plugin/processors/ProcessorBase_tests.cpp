#if ! LEMONS_UNIT_TESTS
#  error
#endif


namespace lemons::tests
{

static AudioProcessorTest<plugin::BasicProcessor> basicProcessorTest { "BasicProcessor tests" };


class ProcessorBaseTest final : public AudioProcessorTestBase
{
public:
	explicit ProcessorBaseTest()
	    : AudioProcessorTestBase (processor, "ProcessorBase")
	{
	}

private:

	plugin::State state;

	dsp::PassThroughEngine<float>  floatEngine;
	dsp::PassThroughEngine<double> doubleEngine;

	plugin::ProcessorBase processor { floatEngine, doubleEngine, state };
};

static ProcessorBaseTest processorBaseTest;

}  // namespace lemons::tests
