#if ! LEMONS_UNIT_TESTS
#	error
#endif

namespace lemons::tests
{

struct ParameterTests final : public Test
{
	explicit ParameterTests()
		: Test ("Parameter base class test", "Plugins")
	{
		processor.addParameter (parameter.release());
	}

private:

	void runTest() final
	{
		beginTest ("Parameter tests");

		auto* p = processor.getParameter (parameterName);

		if (p == nullptr)
			expect (false);

		auto* d = dynamic_cast<plugin::Parameter*> (p);

		if (d == nullptr)
			expect (false);

		auto& param = *d;

		{
			const auto subtest = beginSubtest ("ParameterTraits");

			const auto& traits = param.getParameterTraits();

			{
				const auto subtest = beginSubtest ("Create traits from parameter");

				expect (traits.isValid());

				expectEquals (traits.name, param.getName (50));

				expectEquals (traits.label, param.getLabel());

				{
					const auto& range = param.getNormalisableRange();

					expectEquals (traits.range.start, range.start);
					expectEquals (traits.range.end, range.end);
					expectEquals (traits.range.interval, range.interval);
					expectEquals (traits.range.skew, range.skew);
					//        expectEquals (traits.range.symmetricSkew, range.symmetricSkew);
				}

				expectEquals (traits.defaultValue, param.getNormalizedDefault());

				//    expectEquals (traits.isAutomatable, param.isAutomatable());

				//    expectEquals (traits.isMetaParameter, param.isMetaParameter());

				expectEquals (traits.category, param.getCategory());
			}
			{
				const auto subtest = beginSubtest ("Create parameter from traits");

				plugin::ParameterList dummyList;

				const auto parameter_ = traits.createParameter (dummyList);

				expectEquals (parameter_->getName (50), traits.name);

				expectEquals (parameter_->getLabel(), traits.label);

				{
					const auto& range = parameter_->getNormalisableRange();

					expectEquals (range.start, traits.range.start);
					expectEquals (range.end, traits.range.end);
					expectEquals (range.interval, traits.range.interval);
					expectEquals (range.skew, traits.range.skew);
					//        expectEquals (range.symmetricSkew, traits.range.symmetricSkew);
				}

				expectEquals (parameter_->getNormalizedDefault(), traits.defaultValue);

				//    expectEquals (parameter_->isAutomatable(), traits.isAutomatable);

				//    expectEquals (parameter_->isMetaParameter(), traits.isMetaParameter);

				expectEquals (parameter_->getCategory(), traits.category);
			}
		}

		{
			const auto subtest = beginSubtest ("Value");

			// set norm value
			// set denorm value
		}

		{
			const auto subtest = beginSubtest ("Default value");

			// set norm default
			// set denorm default
			// refresh default
			// reset to default
		}

		{
			const auto subtest = beginSubtest ("Controller messages");

			// assign mapping
			// change mapping
			// process CC messages
		}

		{
			const auto subtest = beginSubtest ("Gesture state");

			// begin gesture
			// is changing
			// end gesture
		}

		{
			const auto subtest = beginSubtest ("State saving and loading");

			// load and save from value tree
		}
	}

	static constexpr auto parameterName = "TestParameter";

	std::unique_ptr<plugin::Parameter> parameter { std::make_unique<plugin::Parameter> (parameterName,
		ranges::create (0.f, 100.f),
		35.f,
		nullptr, nullptr,
		"test_units",
		true, false) };
	;

	plugin::BasicProcessor processor;
};

static ParameterTests parameterTests;

}  // namespace lemons::tests
