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
        {
            beginTest ("Loading images from binary data");
            
            const juce::StringArray imageNames { "deke.png", "lemons.jpg", "logo.png" };
            
            for (const auto filename : imageNames)
            {
                const auto image = binary::getImage (filename);
                
                expect (image.isValid());
            }
            
            {
                const auto subtest = beginSubtest ("Getting image file names");
                
                const auto imageNames_ = binary::getImageFileNames();
                
                expect (imageNames_.size() == imageNames.size());
                
                for (const auto& name : imageNames)
                    expect (imageNames_.contains (name));
            }
        }
        
        
		beginTest ("GUI serializing");

		// Image image { juce::Image::PixelFormat::RGB, 250, 250, true };

		// fillImageWithRandomPixels (image, getRandom());

		// const auto blob         = serializing::imageToBinary (image);
		// const auto decodedImage = serializing::imageFromBinary (blob);

		//    expect (imagesAreEqual (decodedImage, image));

		auto rand = getRandom();

		{
			const auto subtest = beginSubtest ("Colour");

			juce::Colour orig { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

			expect (orig == toVarAndBack (orig));
		}

		{
			// const auto subtest = beginSubtest ("Image");

			// Image orig { juce::Image::PixelFormat::RGB, 250, 250, true };

			// fillImageWithRandomPixels (orig, rand);

			//        expect (imagesAreEqual (orig, toVarAndBack (orig)));
		}

		{
			const auto subtest = beginSubtest ("Justification");

			using juce::Justification;

			Justification orig { Justification::Flags::left };

			expect (orig == toVarAndBack (orig));
		}
	}
};

static GuiSerializingTests guiSerializingTest;

}  // namespace lemons::tests
