
#pragma once

namespace lemons
{

void initializeTranslations (const File& translationFile, bool ignoreCaseOfKeys = true);

void initializeTranslations (const binary::Data& data, bool ignoreCaseOfKeys = true);


void initializeDefaultTranslations();


struct TranslationsInitializer
{
	explicit TranslationsInitializer();

	virtual ~TranslationsInitializer() = default;
};

}  // namespace lemons
