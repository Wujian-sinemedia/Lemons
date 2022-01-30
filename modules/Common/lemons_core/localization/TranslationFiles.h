/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

#pragma once

namespace lemons::locale
{

using juce::File;


/** @name lemons_translation_file_generation Translation file generation
	@ingroup lemons_locale
 */
///@{

/** Writes a JUCE-style translation file header, including the language name and relevant country codes, for the specified language.
	@param language The language name or language code. When interpreting this argument as a language code, a 2-character string will be interpreted as an ISO639-1 code; if it is 3 characters long, it will be interpreted as an ISO639-2 code. Other lengths of passed strings are invalid and will cause this method to return an empty string.
	@param languageName If true, the 'language' argument is interpreted as the name of the language; if false, it is interpreted as an ISO language code.
 */
[[nodiscard]] String writeTranslationFileHeader (const String& language, bool languageName = true);


/** Recursively scans a directory tree for instances of juce::translate or the TRANS macro, called with raw string literals, and returns a list of all the string literals that need to be translated.
 */
[[nodiscard]] StringArray getPhrasesToBeTranslated (const File& rootDir);


/** Scans a directory tree for phrases that need to be translated, and generates JUCE-style translation files for each specified language, placing them in the output directory.
 */
void generateTranslationFiles (const File&		  rootDir,
							   const StringArray& languageCodes,
							   const File& outputDir, const String& filenamePrefix = "trans_", const String& fileExtension = ".txt");

///@}

}  // namespace lemons::locale
