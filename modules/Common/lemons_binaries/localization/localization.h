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

#pragma once

namespace lemons::locale
{

/** Initializes the juce::LocalisedStrings translation system from a translation file contained in a binary::Data object. */
void initializeTranslations (const binary::Data& data, bool ignoreCaseOfKeys = true);


/** Attempts to load a set of default translations from binary data, chosen based on the user's display language settings. */
void initializeDefaultTranslations();


/** A struct whose constructor simply calls initializeDefaultTranslations().
    This is useful for initializing the translations system as part of a base class, before the other members of your class are created; for example:
    @code
    struct MyProcessor : public juce::AudioProcessor, private lemons::locale::TranslationsInitializer
    {
        MyProcessor()
        {
            // parameters you create in the constructor can now call juce::translate() or TRANS() to internationalize their display names!
            // the translation system will be initialized before you enter the body of this constructor!
        }
    };
    @endcode
 */
struct TranslationsInitializer
{
	/** The constructor simply calls initializeDefaultTranslations(). */
	explicit TranslationsInitializer();

	/** Destructor. */
	virtual ~TranslationsInitializer() = default;
};

}  // namespace lemons::locale
