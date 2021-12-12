#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename Type>
[[nodiscard]] inline Type toVarAndBack (const Type& orig)
{
	using Converter = juce::VariantConverter<Type>;

	return Converter::fromVar (Converter::toVar (orig));
}


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

		const auto decoded = toVarAndBack (params);

		expectEquals (decoded.attack, params.attack);
		expectEquals (decoded.decay, params.decay);
		expectEquals (decoded.sustain, params.sustain);
		expectEquals (decoded.release, params.release);
	}

	{
		//        const auto subtest = beginSubtest ("Audio buffers");
		//
		//        runAudioBufferTest<float>();
		//        runAudioBufferTest<double>();
	}

	{
		const auto subtest = beginSubtest ("BigInteger");

		const BigInteger orig { rand.nextInt64() };

		expect (toVarAndBack (orig) == orig);
	}

	{
		const auto subtest = beginSubtest ("Colour");

		const Colour orig { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

		expect (orig == toVarAndBack (orig));
	}

	{
		const auto subtest = beginSubtest ("Identifier");

		const Identifier orig { "TestID" };

		expect (orig == toVarAndBack (orig));
	}

	{
		//        const auto subtest = beginSubtest ("Image");
		//
		//        Image orig { juce::Image::PixelFormat::RGB, 250, 250, true };
		//
		//        fillImageWithRandomPixels (orig, rand);
		//
		//        expect (imagesAreEqual (orig, toVarAndBack (orig)));
	}

	{
		const auto subtest = beginSubtest ("IPAddress");

		const auto orig = IPAddress::getLocalAddress();

		expect (orig == toVarAndBack (orig));
	}

	{
		const auto subtest = beginSubtest ("Justification");

		const Justification orig { Justification::Flags::left };

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
		//        const auto subtest = beginSubtest ("MidiBuffer");
		//
		//        MidiBuffer orig;
		//
		//        fillMidiBufferWithRandomEvents (orig, rand.nextInt ({ 1, 100 }), rand);
		//
		//        expect (midiBuffersAreEqual (orig, toVarAndBack (orig)));
	}

	//    {
	//        const auto subtest = beginSubtest ("MidiFile");
	//    }

	{
		const auto subtest = beginSubtest ("MidiMessage");

		constexpr auto channel  = 1;
		constexpr auto note     = 78;
		constexpr auto velocity = 0.38f;

		const auto orig = MidiMessage::noteOn (channel, note, velocity);

		const auto decoded = toVarAndBack (orig);

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

		expect (orig == toVarAndBack (orig));
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

template <typename SampleType>
void VariantConversionTests::runAudioBufferTest()
{
	const auto subtest = beginSubtest (getPrecisionString<SampleType>() + " precision tests");

	AudioBuffer<SampleType> orig { 2, 256 };

	fillAudioBufferWithRandomNoise (orig, getRandom());

	expect (buffersAreReasonablyEqual (orig, toVarAndBack (orig)));
}

template void VariantConversionTests::runAudioBufferTest<float>();
template void VariantConversionTests::runAudioBufferTest<double>();

}  // namespace lemons::tests

#endif
