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

namespace lemons
{

using juce::File;

void initializeTranslations (const File& translationFile, bool ignoreCaseOfKeys = true);

void initializeTranslations (const binary::Data& data, bool ignoreCaseOfKeys = true);


void initializeDefaultTranslations();


struct TranslationsInitializer
{
	explicit TranslationsInitializer();

	virtual ~TranslationsInitializer() = default;
};

}  // namespace lemons
