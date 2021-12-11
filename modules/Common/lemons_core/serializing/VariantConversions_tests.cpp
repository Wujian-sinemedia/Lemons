#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

VariantConversionTests::VariantConversionTests()
    : CoreTest ("Variant conversions")
{
}

void VariantConversionTests::runTest()
{
	using namespace juce;

	auto rand = getRandom();

	beginTest ("Variant conversions - JUCE classes");

	{
		const auto subtest = beginSubtest ("ADSR::Parameters");

		const ADSR::Parameters params { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

		const auto var     = VariantConverter<ADSR::Parameters>::toVar (params);
		const auto decoded = VariantConverter<ADSR::Parameters>::fromVar (var);

		expectEquals (decoded.attack, params.attack);
		expectEquals (decoded.decay, params.decay);
		expectEquals (decoded.sustain, params.sustain);
		expectEquals (decoded.release, params.release);
	}

	{
		//        const auto bufferSubtest = beginSubtest ("Audio buffers");
		//
		//        {
		//            const auto subtest = beginSubtest ("float");
		//        }
		//        {
		//            const auto subtest = beginSubtest ("double");
		//        }
	}

	{
		const auto subtest = beginSubtest ("BigInteger");

		const BigInteger orig { rand.nextInt64() };

		const auto var     = VariantConverter<BigInteger>::toVar (orig);
		const auto decoded = VariantConverter<BigInteger>::fromVar (var);

		expect (decoded == orig);
	}

	{
		const auto subtest = beginSubtest ("Colour");

		const Colour orig { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

		const auto var     = VariantConverter<Colour>::toVar (orig);
		const auto decoded = VariantConverter<Colour>::fromVar (var);

		expect (orig == decoded);
	}

	{
		const auto subtest = beginSubtest ("Identifier");

		const Identifier orig { "TestID" };

		const auto var     = VariantConverter<Identifier>::toVar (orig);
		const auto decoded = VariantConverter<Identifier>::fromVar (var);

		expect (orig == decoded);
	}

	//    {
	//        const auto subtest = beginSubtest ("Image");
	//    }

	{
		const auto subtest = beginSubtest ("IPAddress");

		const auto orig = IPAddress::getLocalAddress();

		const auto var     = VariantConverter<IPAddress>::toVar (orig);
		const auto decoded = VariantConverter<IPAddress>::fromVar (var);

		expect (orig == decoded);
	}

	{
		const auto subtest = beginSubtest ("Justification");

		const Justification orig { Justification::Flags::left };

		const auto var     = VariantConverter<Justification>::toVar (orig);
		const auto decoded = VariantConverter<Justification>::fromVar (var);

		expect (orig == decoded);
	}

	{
		const auto subtest = beginSubtest ("MAC Address");

		for (const auto& orig : MACAddress::getAllAddresses())
		{
			const auto var     = VariantConverter<MACAddress>::toVar (orig);
			const auto decoded = VariantConverter<MACAddress>::fromVar (var);

			expect (orig == decoded);
		}
	}

	//    {
	//        const auto subtest = beginSubtest ("MemoryBlock");
	//    }

	//    {
	//        const auto subtest = beginSubtest ("MidiBuffer");
	//    }
	//
	//    {
	//        const auto subtest = beginSubtest ("MidiFile");
	//    }

	{
		const auto subtest = beginSubtest ("MidiMessage");

		constexpr auto channel  = 1;
		constexpr auto note     = 78;
		constexpr auto velocity = 0.38f;

		const auto orig = MidiMessage::noteOn (channel, note, velocity);

		const auto var     = VariantConverter<MidiMessage>::toVar (orig);
		const auto decoded = VariantConverter<MidiMessage>::fromVar (var);

		expect (decoded.isNoteOn());

		expectEquals (decoded.getChannel(), channel);
		expectEquals (decoded.getNoteNumber(), note);

		expectWithinAbsoluteError (decoded.getFloatVelocity(), velocity, 0.01f);
	}

	{
		const auto subtest = beginSubtest ("NamedValueSet");

		NamedValueSet orig;

		orig.set ("String", "Apple");
		orig.set ("Integer", 42);
		orig.set ("Float", 38.13f);

		const auto var     = VariantConverter<NamedValueSet>::toVar (orig);
		const auto decoded = VariantConverter<NamedValueSet>::fromVar (var);

		expect (decoded == orig);
	}

	{
		const auto subtest = beginSubtest ("RelativeTime");

		const auto orig = RelativeTime::milliseconds (rand.nextInt64());

		const auto var     = VariantConverter<RelativeTime>::toVar (orig);
		const auto decoded = VariantConverter<RelativeTime>::fromVar (var);

		expectEquals (decoded.inMilliseconds(), orig.inMilliseconds());
	}

	{
		const auto subtest = beginSubtest ("StringArray");

		const StringArray orig { "String1", "String2", "String3", "String4" };

		const auto var     = VariantConverter<StringArray>::toVar (orig);
		const auto decoded = VariantConverter<StringArray>::fromVar (var);

		expect (decoded == orig);
	}

	{
		const auto subtest = beginSubtest ("StringPairArray");

		StringPairArray orig;

		orig.set ("String1", "Apple");
		orig.set ("Fruit", "Orange");
		orig.set ("Veggie", "Celery");

		const auto var     = VariantConverter<StringPairArray>::toVar (orig);
		const auto decoded = VariantConverter<StringPairArray>::fromVar (var);

		expect (orig == decoded);
	}

	{
		const auto subtest = beginSubtest ("Time");

		const auto orig = Time::getCurrentTime();

		const auto var     = VariantConverter<Time>::toVar (orig);
		const auto decoded = VariantConverter<Time>::fromVar (var);

		expectEquals (decoded.toMilliseconds(), orig.toMilliseconds());
	}

	{
		const auto subtest = beginSubtest ("UUID");

		const Uuid orig;

		const auto var     = VariantConverter<Uuid>::toVar (orig);
		const auto decoded = VariantConverter<Uuid>::fromVar (var);

		expect (orig == decoded);
	}

	{
		const auto subtest = beginSubtest ("URL");

		const URL orig { "www.juce.com" };

		const auto var     = VariantConverter<URL>::toVar (orig);
		const auto decoded = VariantConverter<URL>::fromVar (var);

		expect (orig == decoded);
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

		const auto var     = VariantConverter<Dimensions>::toVar (orig);
		const auto decoded = VariantConverter<Dimensions>::fromVar (var);

		expect (orig == decoded);
	}

	{
		using lemons::Version;

		const auto subtest = beginSubtest ("Version");

		constexpr auto orig = Version::juceVersion();

		const auto var     = VariantConverter<Version>::toVar (orig);
		const auto decoded = VariantConverter<Version>::fromVar (var);

		expect (decoded == orig);
	}
}

}  // namespace lemons::tests

#endif
