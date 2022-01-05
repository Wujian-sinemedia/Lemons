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

namespace lemons::locale
{

String writeTranslationFileHeader (const String& language, bool languageName)
{
	const auto language_name = [&]() -> String
	{
		if (languageName)
			return language;

		return languageCodeToName (language);
	}();

	if (language_name.isEmpty())
		return {};

	String header;

	header << "language: " << language_name << juce::newLine;

	const auto countries = getCountryCodesForLanguage (language_name);

	if (! countries.isEmpty())
		header << "countries: " << countries.joinIntoString (" ") << juce::newLine;

	return header;
}


[[nodiscard]] StringArray scanFileForPhrases (const File& file)
{
	if (! file.existsAsFile())
		return {};

	static const StringArray stringsToFind { "translate", "TRANS" };

	StringArray fileLines;
	file.readLines (fileLines);

	StringArray phrases;

	for (const auto& line : fileLines)
	{
		if (line.isEmpty())
			continue;

		const auto lineLength = line.length();

		for (const auto& stringToFind : stringsToFind)
		{
			int idx { 0 };

			while (idx < lineLength)
			{
				const auto begin = line.indexOfWholeWord (stringToFind);

				if (begin == -1)
					break;

				const auto restOfString = line.substring (idx + stringToFind.length());

				const auto firstQuoteIdx = restOfString.indexOfChar ('"');

				if (firstQuoteIdx == -1)
					break;

				const auto afterFirstQuote = restOfString.substring (firstQuoteIdx + 1);

				const auto endIdx = afterFirstQuote.indexOfChar ('"');

				if (endIdx == -1)
					break;

				phrases.add (afterFirstQuote.substring (0, endIdx));

				idx = endIdx + 1;
			}
		}
	}

	return phrases;
}

StringArray getPhrasesToBeTranslated (const File& rootDir)
{
	if (! rootDir.isDirectory())
		return {};

	StringArray phrases;

	for (const auto& entry : juce::RangedDirectoryIterator { rootDir, true })
	{
		if (entry.isDirectory())
			phrases.addArray (getPhrasesToBeTranslated (entry.getFile()));
		else
			phrases.addArray (scanFileForPhrases (entry.getFile()));
	}

	phrases.removeDuplicates (true);
	phrases.removeEmptyStrings();
	phrases.trim();

	phrases.sortNatural();

	return phrases;
}


void generateTranslationFiles (const File& rootDir, const StringArray& languageCodes,
                               const File& outputDir, const String& filenamePrefix, const String& fileExtension)
{
	String fileBody;

	for (const auto& phrase : getPhrasesToBeTranslated (rootDir))
		fileBody << phrase.quoted() << " = \"\"" << juce::newLine;


	for (const auto& language : languageCodes)
	{
		const auto languageName = languageCodeToName (language);

		auto fileText = writeTranslationFileHeader (languageName, true);

		if (fileText.isEmpty())
			continue;

		const auto outFile = outputDir.getChildFile (filenamePrefix + languageName + fileExtension);

		outFile.deleteFile();

		fileText << juce::newLine << juce::newLine << fileBody;

		outFile.replaceWithText (fileText);
	}
}

}  // namespace lemons::locale
