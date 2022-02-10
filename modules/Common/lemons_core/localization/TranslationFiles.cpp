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
 * ======================================================================================
 */

namespace lemons::locale
{

String writeTranslationFileHeader (const String& language, bool languageName)
{
	const auto language_name = [languageName, &language]
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
	else
		header << "countries: [write list of country codes for this language here]" << juce::newLine;

	return header;
}


[[nodiscard]] inline juce_wchar readEscapedChar (String::CharPointerType& p) noexcept
{
	auto c = *p;

	switch (c)
	{
		case '"' : break;
		case '\\' : break;
		case '/' : break;

		case 'b' : c = '\b'; break;
		case 'f' : c = '\f'; break;
		case 'n' : c = '\n'; break;
		case 'r' : c = '\r'; break;
		case 't' : c = '\t'; break;

		case 'x' :
			++p;
			c = 0;

			for (auto i = 4; --i >= 0;)
			{
				const auto digitValue = juce::CharacterFunctions::getHexDigitValue (*p);

				if (digitValue < 0)
					break;

				++p;
				c = (c << 4) + (juce_wchar) digitValue;	 // NOLINT
			}

			break;

		case '0' : [[fallthrough]];
		case '1' : [[fallthrough]];
		case '2' : [[fallthrough]];
		case '3' : [[fallthrough]];
		case '4' : [[fallthrough]];
		case '5' : [[fallthrough]];
		case '6' : [[fallthrough]];
		case '7' : [[fallthrough]];
		case '8' : [[fallthrough]];
		case '9' :
			c = 0;

			for (auto i = 4; --i >= 0;)
			{
				const auto digitValue = (int) (*p - '0');  // NOLINT

				if (digitValue < 0 || digitValue > 7)
					break;

				++p;
				c = (c << 3) + (juce_wchar) digitValue;	 // NOLINT
			}

			break;

		default :
			break;
	}

	return c;
}


inline void parseStringLiteral (String::CharPointerType& p, juce::MemoryOutputStream& out) noexcept
{
	p.incrementToEndOfWhitespace();

	if (p.getAndAdvance() == '"')
	{
		auto start = p;

		do
		{
			auto c = *p;

			if (c == '"')
			{
				out << String (start, p);
				++p;
				parseStringLiteral (p, out);
				return;
			}

			if (c == 0)
				break;

			if (c == '\\')
			{
				out << String (start, p);
				++p;
				out << String::charToString (readEscapedChar (p));
				start = p + 1;
			}

			++p;
		} while (true);
	}
}


[[nodiscard]] StringArray scanFileForPhrases (const File& file)
{
	if (! file.existsAsFile())
		return {};

	if (! file.hasFileExtension ("h;hpp;c;cpp;mm"))
		return {};

	StringArray strings;

	auto content = file.loadFileAsString();
	auto p		 = content.getCharPointer();

	do
	{
		p = juce::CharacterFunctions::find (p, juce::CharPointer_ASCII ("TRANS"));

		if (p.isEmpty())
			break;

		p += 5;
		p.incrementToEndOfWhitespace();

		if (*p == '(')
		{
			++p;
			juce::MemoryOutputStream text;
			parseStringLiteral (p, text);

			const auto s = text.toString();

			if (s.isNotEmpty() && ! strings.contains (s))
				strings.add (s);
		}
	} while (true);

	return strings;
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
