#if ! LEMONS_UNIT_TESTS
#  error
#endif


namespace lemons::tests
{

struct GuiSerializingTests final : public CoreTest
{
	explicit GuiSerializingTests()
	    : CoreTest ("GUI Serializing")
	{
	}

private:
	void runTest() final
	{
		Image image { juce::Image::PixelFormat::RGB, 250, 250, true };

		fillImageWithRandomPixels (image, getRandom());

		const auto blob         = serializing::imageToBinary (image);
		const auto decodedImage = serializing::imageFromBinary (blob);

		//    expect (imagesAreEqual (decodedImage, image));

		auto rand = getRandom();

		{
			const auto subtest = beginSubtest ("Colour");

			const juce::Colour orig { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

			expect (orig == toVarAndBack (orig));
		}

		{
			const auto subtest = beginSubtest ("Image");

			Image orig { juce::Image::PixelFormat::RGB, 250, 250, true };

			fillImageWithRandomPixels (orig, rand);

			//        expect (imagesAreEqual (orig, toVarAndBack (orig)));
		}

		{
			const auto subtest = beginSubtest ("Justification");

			using juce::Justification;

			const Justification orig { Justification::Flags::left };

			expect (orig == toVarAndBack (orig));
		}
	}
};

static GuiSerializingTests guiSerializingTest;

}  // namespace lemons::tests
