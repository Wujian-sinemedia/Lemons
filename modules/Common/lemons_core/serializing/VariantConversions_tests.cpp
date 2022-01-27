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


#if ! LEMONS_UNIT_TESTS
#	error
#endif

namespace lemons::tests
{

struct VariantConversionTests final : public CoreTest
{
	explicit VariantConversionTests()
		: CoreTest ("Variant conversions")
	{
	}

private:

	void runTest() final
	{
		using namespace juce;

		auto rand = getRandom();

		beginTest ("Variant conversions - JUCE classes");

		{
			const auto subtest = beginSubtest ("BigInteger");

			const BigInteger orig { rand.nextInt64() };

			expect (toVarAndBack (orig) == orig);
		}

		{
			const auto subtest = beginSubtest ("Identifier");

			const Identifier orig { "TestID" };

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("IPAddress");

			const auto orig = IPAddress::getLocalAddress();

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("MAC Address");

			for (const auto& orig : MACAddress::getAllAddresses())
				expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("MemoryBlock");

			MemoryBlock orig { size_t (512) };

			orig.fillWith (uint8 (17));

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("NamedValueSet");

			NamedValueSet orig;

			orig.set ("String", "Apple");
			orig.set ("Integer", 42);
			orig.set ("Float", 38.13f);

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("PropertySet");

			PropertySet set;

			set.setValue ("A_string", "Apple");
			set.setValue ("A_number", 42);
			set.setValue ("A_double", 36.18);

			auto decoded = toVarAndBack (set);
			expect (set.getAllProperties() == decoded.getAllProperties());
		}

		{
			const auto subtest = beginSubtest ("RelativeTime");

			const auto orig = RelativeTime::milliseconds (rand.nextInt ({ 1, 10000 }));

			const auto decoded = toVarAndBack (orig);

			expectEquals (decoded.inMilliseconds(), orig.inMilliseconds());
		}

		{
			const auto subtest = beginSubtest ("StringArray");

			const StringArray orig { "String1", "String2", "String3", "String4" };

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("StringPairArray");

			StringPairArray orig;

			orig.set ("String1", "Apple");
			orig.set ("Fruit", "Orange");
			orig.set ("Veggie", "Celery");

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("Time");

			const auto orig = Time::getCurrentTime();

			const auto decoded = toVarAndBack (orig);

			expectEquals (decoded.toMilliseconds(), orig.toMilliseconds());
		}

		{
			const auto subtest = beginSubtest ("UUID");

			const Uuid orig;

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("URL");

			const URL orig { "www.juce.com" };

			expect (orig == toVarAndBack (orig));
		}

		//    {
		//        const auto subtest = beginSubtest ("ValueTree");
		//    }

		/*---------------------------------------------------------------------------------------------------------------------------------*/

		beginTest ("Variant conversions - Lemons classes");

		{
			using lemons::Dimensions;

			const auto subtest = beginSubtest ("Dimensions");

			constexpr auto orig = Dimensions::getDefault();

			expect (orig == toVarAndBack (orig));
		}

		{
			using lemons::Version;

			const auto subtest = beginSubtest ("Version");

			constexpr auto orig = Version::juceVersion();

			expect (orig == toVarAndBack (orig));
		}
	}
};

static VariantConversionTests variantConverterTest;


}  // namespace lemons::tests
