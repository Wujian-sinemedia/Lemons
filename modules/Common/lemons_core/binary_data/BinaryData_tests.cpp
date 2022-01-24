#if ! LEMONS_UNIT_TESTS
#  error
#endif

static_assert (lemons::binary::hasBinaryData(), "Must have binary data to test the binary data!");


namespace lemons::tests
{

struct BinaryDataTests final : public Test
{
	explicit BinaryDataTests()
		: Test ("Binary data tests", "BinaryData")
	{
	}

private:
	void runTest() final
	{
		beginTest ("binary::Data objects are valid");

		{
			const auto subtest = beginSubtest ("explicit char pointer constructor");

			const binary::Data data { "deke.png" };

			expect (data.isValid());
		}

		{
			const auto subtest = beginSubtest ("juce String constructor");

			for (const auto& filename : binary::getFilenames())
			{
				const binary::Data data { filename };

				expect (data.isValid());
			}
		}

		{
			const auto subtest = beginSubtest ("Retrieving data");

			for (const auto& filename : binary::getFilenames())
			{
				const binary::Data data { filename };

				expect (data.isValid());

				expect (data.getAsString().isNotEmpty());

				const auto block = data.getAsMemoryBlock();

				expect (! block.isEmpty());
				expect (block.getSize() == static_cast<size_t> (data.getSize()));
			}
		}

		const auto subtest = beginSubtest ("Free functions for retrieving data");

		for (const auto& filename : binary::getFilenames())
		{
			expect (! binary::getBlob (filename).isEmpty());
			expect (! binary::getString (filename).isEmpty());
			expect (! binary::getStrings (filename).isEmpty());

			// TO DO: getValueTree()
		}
	}
};

static BinaryDataTests binaryDataTests;

}  // namespace lemons::tests
