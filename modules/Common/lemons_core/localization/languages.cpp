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

struct LanguageData final
{
	LanguageData()
		: languageName (UNKNOWN)
	{
	}

	LanguageData (const String& name,
				  const String& ISO_639_1, const String& ISO_639_2,
				  const StringArray& countries = {})
		: languageName (name), ISO639_1 (ISO_639_1), ISO639_2 (ISO_639_2), countryCodes (countries)
	{
		jassert (ISO639_1.isEmpty() || ISO639_1.length() == 2);
		jassert (ISO639_2.length() == 3);
	}

	[[nodiscard]] bool isValid() const
	{
		return languageName != UNKNOWN && (ISO639_1.isNotEmpty() || ISO639_2.isNotEmpty());
	}

	String languageName, ISO639_1, ISO639_2;

	StringArray countryCodes;

private:

	static constexpr auto UNKNOWN = "Unknown_language_";
};


/** Note that ISO639-2/B codes are used instead of ISO639-2/T codes.
	See https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
	And https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
 */
struct KnownLanguages final
{
	KnownLanguages()
	{
		using juce::CharPointer_UTF8;

		languages.emplace_back ("Abkhazian", "ab", "abk", StringArray { "GE" });
		languages.emplace_back ("Achinese", "", "ace", StringArray { "ID" });
		languages.emplace_back ("Acoli", "", "ach", StringArray { "UG" });
		languages.emplace_back ("Adangme", "", "ada", StringArray { "GH" });
		languages.emplace_back ("Adyghe", "", "ady", StringArray { "RU" });
		languages.emplace_back ("Afar", "aa", "aar", StringArray { "DJ", "ER", "ET" });
		languages.emplace_back ("Afrikaans", "af", "afr", StringArray { "NA", "ZA" });
		languages.emplace_back ("Ainu", "", "ain", StringArray { "RU", "JP" });
		languages.emplace_back ("Akan", "ak", "aka", StringArray { "CI", "GH" });
		languages.emplace_back ("Albanian", "sq", "alb", StringArray { "AL", "IT", "MK", "ME", "RO" });
		languages.emplace_back ("Aleut", "", "ale", StringArray { "US", "RU" });
		languages.emplace_back ("Amharic", "am", "amh", StringArray { "ET" });
		languages.emplace_back ("Angika", "", "anp", StringArray { "IN", "NP" });
		languages.emplace_back ("Arabic", "ar", "ara",
								StringArray { "DZ", "BH", "BJ", "TD", "KM", "DJ", "EG", "ER", "GM", "GN", "GW", "ID", "IR", "IL", "JO", "KW", "LB", "LY", "ML", "MR", "MA", "NE", "OM", "PS", "PH", "QA", "SA", "SN", "SO", "ZA", "SD", "SY", "TJ", "TZ", "TN", "AE", "UZ", "VE", "EH", "YE" });
		languages.emplace_back ("Aragonese", "an", "arg", StringArray { "ES" });
		languages.emplace_back ("Arapaho", "", "arp", StringArray { "US" });
		languages.emplace_back ("Arawak", "", "arw", StringArray { "SR", "VE", "GY" });
		languages.emplace_back ("Armenian", "hy", "arm", StringArray { "AM", "CY", "IQ", "RO" });
		languages.emplace_back ("Aromanian", "", "rup", StringArray { "MK" });
		languages.emplace_back ("Assamese", "as", "asm", StringArray { "IN" });
		languages.emplace_back ("Avaric", "av", "ava", StringArray { "RU" });
		languages.emplace_back ("Awadhi", "", "awa", StringArray { "NP" });
		languages.emplace_back ("Aymara", "ay", "aym", StringArray { "BO", "PE" });
		languages.emplace_back ("Azerbaijani", "az", "aze", StringArray { "AZ", "IR", "KZ", "TM", "UZ" });
		languages.emplace_back ("Balinese", "", "ban", StringArray { "ID" });
		languages.emplace_back ("Baluchi", "", "bal", StringArray { "PK", "TM", "IR" });
		languages.emplace_back ("Bambara", "bm", "bam", StringArray { "ML" });
		languages.emplace_back ("Basa", "", "bas", StringArray { "CM" });
		languages.emplace_back ("Bashkir", "ba", "bak", StringArray { "RU" });
		languages.emplace_back ("Basque", "eu", "baq", StringArray { "ES" });
		languages.emplace_back ("Beja", "", "bej", StringArray { "ER" });
		languages.emplace_back ("Belarusian", "be", "bel", StringArray { "BY", "CZ", "UA" });
		languages.emplace_back ("Bemba", "", "bem", StringArray { "ZM" });
		languages.emplace_back ("Bengali", "bn", "ben", StringArray { "BD", "IN", "ID" });
		languages.emplace_back ("Bhojpuri", "", "bho", StringArray { "NP" });
		languages.emplace_back ("Bikol", "", "bik", StringArray { "PH" });
		languages.emplace_back ("Bini", "", "bin", StringArray { "NG" });
		languages.emplace_back ("Bislama", "bi", "bis", StringArray { "VU" });
		languages.emplace_back ("Bilin", "", "byn", StringArray { "ER" });
		languages.emplace_back (CharPointer_UTF8 ("Bokmål"), "", "nob", StringArray { "NO" });
		languages.emplace_back ("Bosnian", "bs", "bos", StringArray { "BA", "MK", "ME" });
		languages.emplace_back ("Braj", "", "bra", StringArray { "IN" });
		languages.emplace_back ("Breton", "br", "bre", StringArray { "FR" });
		languages.emplace_back ("Buginese", "", "bug", StringArray { "ID" });
		languages.emplace_back ("Bulgarian", "bg", "bul", StringArray { "BG", "RO", "UA" });
		languages.emplace_back ("Buriat", "", "bua", StringArray { "RU" });
		languages.emplace_back ("Burmese", "my", "bur", StringArray { "MM" });
		languages.emplace_back ("Caddo", "", "cad", StringArray { "US" });
		languages.emplace_back ("Catalan", "ca", "cat", StringArray { "AD", "IT", "ES" });
		languages.emplace_back ("Cebuano", "", "ceb", StringArray { "PH" });
		languages.emplace_back ("Central Khmer", "", "khm", StringArray { "KH" });
		languages.emplace_back ("Chamorro", "ch", "cha", StringArray { "GU", "MP" });
		languages.emplace_back ("Chechen", "ce", "che", StringArray { "RU" });
		languages.emplace_back ("Cherokee", "", "chr", StringArray { "US" });
		languages.emplace_back ("Cheyenne", "", "chy", StringArray { "US" });
		languages.emplace_back ("Chibcha", "", "chb", StringArray { "CO" });
		languages.emplace_back ("Chichewa", "ny", "nya", StringArray { "MW", "MZ", "ZM", "ZW" });
		languages.emplace_back ("Chinese", "zh", "chi", StringArray { "BZ", "BN", "CN", "GY", "HK", "MO", "SR", "TW", "VE" });
		languages.emplace_back ("Chinook jargon", "", "chn", StringArray { "US" });
		languages.emplace_back ("Chipewyan", "", "chp", StringArray { "CA" });
		languages.emplace_back ("Choctaw", "", "cho", StringArray { "US" });
		languages.emplace_back ("Chuukese", "", "chk", StringArray { "FM" });
		languages.emplace_back ("Chuvash", "cv", "chv", StringArray { "RU" });
		languages.emplace_back ("Cornish", "kw", "cor", StringArray { "GB" });
		languages.emplace_back ("Corsican", "co", "cos", StringArray { "FR", "IT" });
		languages.emplace_back ("Cree", "cr", "cre", StringArray { "CA", "US" });
		languages.emplace_back ("Creek", "", "mus", StringArray { "US" });
		languages.emplace_back ("Crimean Tatar", "", "crh", StringArray { "RO", "RU", "UA" });
		languages.emplace_back ("Croatian", "hr", "hrv", StringArray { "AT", "BA", "HR", "ME", "RO" });
		languages.emplace_back ("Czech", "cs", "cze", StringArray { "CZ", "RO" });
		languages.emplace_back ("Dakota", "", "dak", StringArray { "CA", "US" });
		languages.emplace_back ("Danish", "da", "dan", StringArray { "DK", "FO" });
		languages.emplace_back ("Dargwa", "", "dar", StringArray { "RU" });
		languages.emplace_back ("Delaware", "", "del", StringArray { "CA", "US" });
		languages.emplace_back ("Dinka", "", "din", StringArray { "SS" });
		languages.emplace_back ("Divehi", "dv", "div", StringArray { "MV" });
		languages.emplace_back ("Dogri", "", "doi", StringArray { "IN" });
		languages.emplace_back ("Dogrib", "", "dgr", StringArray { "CA" });
		languages.emplace_back ("Duala", "", "dua", StringArray { "CM" });
		languages.emplace_back ("Dutch", "nl", "dut", StringArray { "AW", "BE", "BQ", "CW", "GY", "ID", "NL", "SX", "SR" });
		languages.emplace_back ("Dyula", "", "dyu", StringArray { "GN", "GM", "CI" });
		languages.emplace_back ("Dzongkha", "dz", "dzo", StringArray { "BT" });
		languages.emplace_back ("Efik", "", "efi", StringArray { "NG" });
		languages.emplace_back ("Ekajuk", "", "eka", StringArray { "NG" });
		languages.emplace_back ("English", "en", "eng",
								StringArray { "AS", "AI", "AG", "AU", "AZ", "BS", "BH", "BB", "BZ", "BJ", "BM", "BQ", "BW", "IO", "BN", "BI", "CM", "CA", "KY", "CX", "CC", "CO", "CK", "CU", "CW", "DM", "ER", "SZ", "FK", "FJ", "FI", "GM", "GH", "GI", "GD", "GU", "GG", "GW", "GN"
																																																																				  "GY",
											  "HK", "IN", "ID", "IQ", "IE", "JM", "KE", "KI", "LS", "LR", "MW", "MY", "MV", "MT", "MH", "MU", "FM", "NA", "NR", "NL", "NZ", "NI", "NG", "NU", "NF", "MP", "PK", "PW", "PG", "PH", "PN", "PR", "QA", "RU", "RW", "KN", "LC", "VC", "WS", "SC", "SL", "SG", "SX", "SB", "SO", "ZA", "GS", "SS", "LK", "SD", "SR", "TZ", "TH", "TL", "TK", "TO", "TT", "TC", "TV", "UG", "GB", "US", "VU", "VE", "VG", "VI", "ZM", "ZW" });
		languages.emplace_back ("Erzya", "", "myv", StringArray { "RU" });
		languages.emplace_back ("Esperanto", "eo", "epo");
		languages.emplace_back ("Estonian", "et", "est", StringArray { "EE" });
		languages.emplace_back ("Ewe", "ee", "ewe", StringArray { "GH", "TG" });
		languages.emplace_back ("Ewondo", "", "ewo", StringArray { "CM" });
		languages.emplace_back ("Fang", "", "fan", StringArray { "GA", "GQ" });
		languages.emplace_back ("Fanti", "", "fat", StringArray { "GH" });
		languages.emplace_back ("Faroese", "fo", "fao", StringArray { "FO" });
		languages.emplace_back ("Fijian", "fj", "fij", StringArray { "FJ" });
		languages.emplace_back ("Filipino", "", "fil", StringArray { "PH" });
		languages.emplace_back ("Finnish", "fi", "fin", StringArray { "FI", "SE" });
		languages.emplace_back ("Fon", "", "fon", StringArray { "BJ" });
		languages.emplace_back ("French", "fr", "fre",
								StringArray { "DZ", "BE", "BJ", "BF", "BI", "CM", "CA", "CF", "TD", "KM", "CD", "CG", "CI", "DJ", "GQ", "FR", "PF", "GM", "GN", "GY", "HT", "VA", "LA", "LB", "LU", "MG", "ML", "MR", "MU", "MC", "MA", "NC", "NE", "RU", "RW", "BL", "MF", "SN", "SC", "SR", "CH", "TG", "TN", "VU", "WF", "EH" });
		languages.emplace_back ("Friulian", "", "fur", StringArray { "IT" });
		languages.emplace_back ("Fulah", "ff", "ful", StringArray { "BJ", "CM", "GM", "GN", "ML", "NE", "NG" });
		languages.emplace_back ("Ga", "", "gaa", StringArray { "GH" });
		languages.emplace_back ("Carib", "", "car", StringArray { "SR", "VE", "GY" });
		languages.emplace_back ("Galician", "gl", "glg", StringArray { "ES" });
		languages.emplace_back ("Ganda", "", "lug", StringArray { "UG" });
		languages.emplace_back ("Gayo", "", "gay", StringArray { "ID" });
		languages.emplace_back ("Gbaya", "", "gba", StringArray { "CD", "CF", "CM", "CG" });
		languages.emplace_back ("Georgian", "ka", "geo", StringArray { "GE" });
		languages.emplace_back ("German", "de", "ger",
								StringArray { "AT", "BE", "BZ", "CR", "CZ", "DK", "DE", "LI", "LU", "NA", "RO", "RU", "ZA", "CH", "VE" });
		languages.emplace_back ("Gilbertese", "", "gil", StringArray { "KI" });
		languages.emplace_back ("Gondi", "", "gon", StringArray { "IN" });
		languages.emplace_back ("Gorontalo", "", "gor", StringArray { "ID" });
		languages.emplace_back ("Grebo", "", "grb", StringArray { "LR" });
		languages.emplace_back ("Greek", "el", "gre", StringArray { "AL", "CY", "CZ", "GR", "RO", "ZA", "UA" });
		languages.emplace_back ("Guarani", "gn", "grn", StringArray { "AR", "BO", "PY" });
		languages.emplace_back ("Gujarati", "gu", "guj", StringArray { "IN", "ID", "ZA" });
		languages.emplace_back ("Gwich'in", "", "gwi", StringArray { "CA", "US" });
		languages.emplace_back ("Haida", "", "hai", StringArray { "CA" });
		languages.emplace_back ("Haitian", "ht", "hat", StringArray { "CU", "HT" });
		languages.emplace_back ("Hausa", "ha", "hau", StringArray { "BJ", "NE", "NG" });
		languages.emplace_back ("Hawaiian", "", "haw", StringArray { "US" });
		languages.emplace_back ("Hebrew", "he", "heb", StringArray { "IL", "ZA", "UA" });
		languages.emplace_back ("Herero", "hz", "her", StringArray { "NA" });
		languages.emplace_back ("Hiligaynon", "", "hil", StringArray { "PH" });
		languages.emplace_back ("Hindi", "hi", "hin", StringArray { "IN", "ZA" });
		languages.emplace_back ("Hiri Motu", "ho", "hmo", StringArray { "PG" });
		languages.emplace_back ("Hmong", "", "hmn", StringArray { "LA" });
		languages.emplace_back ("Hungarian", "hu", "hun", StringArray { "AT", "CZ", "HU", "RO", "SI", "UA" });
		languages.emplace_back ("Hupa", "", "hup");
		languages.emplace_back ("Iban", "", "iba");
		languages.emplace_back ("Iloko", "", "ilo", StringArray { "PH" });
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Ingush", "", "inh", StringArray { "RU" });
		languages.emplace_back ("Interlingua", "ia", "ina");
		languages.emplace_back ("Indonesian", "id", "ind", StringArray { "ID", "TL" });
		languages.emplace_back ("Interlingue", "ie", "ile");
		languages.emplace_back ("Irish", "ga", "gle", StringArray { "IE", "GB" });
		languages.emplace_back ("Igbo", "ig", "ibo", StringArray { "NG" });
		languages.emplace_back ("Inupiaq", "ik", "ipk");
		languages.emplace_back ("Ido", "io", "ido");
		languages.emplace_back ("Icelandic", "is", "ice", StringArray { "IS" });
		languages.emplace_back ("Italian", "it", "ita", StringArray { "CR", "VA", "IT", "MT", "MC", "RO", "SM", "SI", "SO", "CH", "VE" });
		languages.emplace_back ("Inuktitut", "iu", "iku");
		languages.emplace_back ("Inupiaq", "", "ipk");
		languages.emplace_back ("Japanese", "ja", "jpn", StringArray { "ID", "JP", "PW" });
		languages.emplace_back ("Javanese", "jv", "jav", StringArray { "NC", "SR" });
		languages.emplace_back ("Judeo-Arabic", "", "jrb");
		languages.emplace_back ("Judeo-Persian", "", "jpr");
		languages.emplace_back ("Kabardian", "", "kbd", StringArray { "RU" });
		languages.emplace_back ("Kabyle", "", "kab");
		languages.emplace_back ("Kachin", "", "kac", StringArray { "MM" });
		languages.emplace_back ("Kalaallisut", "kl", "kal", StringArray { "GL" });
		languages.emplace_back ("Kalmyk", "", "xal", StringArray { "RU" });
		languages.emplace_back ("Kamba", "", "kam");
		languages.emplace_back ("Kannada", "kn", "kan", StringArray { "IN" });
		languages.emplace_back ("Kanuri", "kr", "kau", StringArray { "NE", "NG" });
		languages.emplace_back ("Kara-Kalpak", "", "kaa", StringArray { "UZ" });
		languages.emplace_back ("Karachay-Balkar", "", "krc", StringArray { "RU" });
		languages.emplace_back ("Karelian", "", "krl", StringArray { "FI" });
		languages.emplace_back ("Kashmiri", "ks", "kas", StringArray { "IN", "PK" });
		languages.emplace_back ("Kashubian", "", "csb");
		languages.emplace_back ("Kawi", "", "kaw");
		languages.emplace_back ("Kazakh", "kk", "kaz", StringArray { "KZ", "TM" });
		languages.emplace_back ("Khasi", "", "kha", StringArray { "IN" });
		languages.emplace_back ("Kikuyu", "", "kik");
		languages.emplace_back ("Kimbundu", "", "kmb", StringArray { "AO" });
		languages.emplace_back ("Kinyarwanda", "", "kin", StringArray { "RW" });
		languages.emplace_back ("Central Khmer", "km", "khm", StringArray { "KH" });
		languages.emplace_back ("Kikuyu", "ki", "kik");
		languages.emplace_back ("Kinyarwanda", "rw", "kin");
		languages.emplace_back ("Kirghiz", "ky", "kir", StringArray { "KG", "TJ" });
		languages.emplace_back ("Komi", "kv", "kom", StringArray { "RU" });
		languages.emplace_back ("Kongo", "kg", "kon", StringArray { "AO" });
		languages.emplace_back ("Konkani", "", "kok", StringArray { "IN" });
		languages.emplace_back ("Korean", "ko", "kor", StringArray { "ID", "KP", "KR" });
		languages.emplace_back ("Kosraean", "", "kos", StringArray { "FM" });
		languages.emplace_back ("Kpelle", "", "kpe", StringArray { "GN" });
		languages.emplace_back ("Kumyk", "", "kum");
		languages.emplace_back ("Kurdish", "ku", "kur", StringArray { "IR", "TM" });
		languages.emplace_back ("Kurukh", "", "kru", StringArray { "IN" });
		languages.emplace_back ("Kutenai", "", "kut");
		languages.emplace_back ("Kuanyama", "kj", "kua", StringArray { "NA" });
		languages.emplace_back ("Luxembourgish", "lb", "ltz", StringArray { "LU" });
		languages.emplace_back ("Ganda", "lg", "lug");
		languages.emplace_back ("Ladino", "", "lad");
		languages.emplace_back ("Lahnda", "", "lah");
		languages.emplace_back ("Lamba", "", "lam", StringArray { "ZM" });
		languages.emplace_back ("Limburgan", "li", "lim");
		languages.emplace_back ("Lingala", "ln", "lin", StringArray { "CD", "CG" });
		languages.emplace_back ("Lao", "lo", "lao", StringArray { "LA" });
		languages.emplace_back ("Lithuanian", "lt", "lit", StringArray { "LT" });
		languages.emplace_back ("Luba-Katanga", "lu", "lub");
		languages.emplace_back ("Luba-Lulua", "", "lua", StringArray { "CD" });
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Lunda", "", "lun", StringArray { "ZM" });
		languages.emplace_back ("Luo", "", "luo");
		languages.emplace_back ("Lushai", "", "lus");
		languages.emplace_back ("Latvian", "lv", "lav", StringArray { "LV" });
		languages.emplace_back ("Lezghian", "", "lez");
		languages.emplace_back ("Lojban", "", "jbo");
		languages.emplace_back ("Low German", "", "nds");
		languages.emplace_back ("Lower Sorbian", "", "dsb");
		languages.emplace_back ("Lozi", "", "loz", StringArray { "NA", "ZM" });
		languages.emplace_back ("Madurese", "", "mad", StringArray { "ID" });
		languages.emplace_back ("Magahi", "", "mag");
		languages.emplace_back ("Maithili", "", "mai", StringArray { "NP", "IN" });
		languages.emplace_back ("Makasar", "", "mak", StringArray { "ID" });
		languages.emplace_back ("Mandingo", "", "man");
		languages.emplace_back ("Masai", "", "mas");
		languages.emplace_back ("Moksha", "", "mdf", StringArray { "RU" });
		languages.emplace_back ("Mandar", "", "mdr");
		languages.emplace_back ("Mende", "", "men");
		languages.emplace_back ("Mi'kmaq", "", "mic");
		languages.emplace_back ("Minangkabau", "", "min", StringArray { "ID" });
		languages.emplace_back ("Manchu", "", "mnc");
		languages.emplace_back ("Manipuri", "", "mni", StringArray { "IN" });
		languages.emplace_back ("Mohawk", "", "moh");
		languages.emplace_back ("Mossi", "", "mos", StringArray { "BJ" });
		languages.emplace_back ("Mirandese", "", "mwl", StringArray { "PT" });
		languages.emplace_back ("Marwari", "", "mwr");
		languages.emplace_back ("Mayan", "", "myn", StringArray { "MX" });
		languages.emplace_back ("Erzya", "", "myv");
		languages.emplace_back ("Nahuatl", "", "nah", StringArray { "MX" });
		languages.emplace_back ("Manx", "gv", "glv", StringArray { "IM" });
		languages.emplace_back ("Macedonian", "mk", "mac", StringArray { "AL", "MK", "RO" });
		languages.emplace_back ("Malagasy", "mg", "mlg", StringArray { "MG" });
		languages.emplace_back ("Malay", "ms", "may", StringArray { "BN", "CX", "CC", "ID", "MY", "SG", "TH" });
		languages.emplace_back ("Malayalam", "ml", "mal", StringArray { "IN" });
		languages.emplace_back ("Maltese", "mt", "mlt", StringArray { "MT" });
		languages.emplace_back ("Maori", "mi", "mao", StringArray { "NZ" });
		languages.emplace_back ("Marathi", "mr", "mar", StringArray { "IN" });
		languages.emplace_back ("Marshallese", "mh", "mah", StringArray { "MH" });
		languages.emplace_back ("Mongolian", "mn", "mon", StringArray { "CN", "MN" });
		languages.emplace_back ("Nauru", "na", "nau", StringArray { "NR" });
		languages.emplace_back ("Navajo", "nv", "nav", StringArray { "US" });
		languages.emplace_back ("Neapolitan", "", "nap");
		languages.emplace_back ("Newari", "", "new", StringArray { "NP" });
		languages.emplace_back ("Nias", "", "nia", StringArray { "ID" });
		languages.emplace_back ("Niuean", "", "niu", StringArray { "NU" });
		languages.emplace_back ("Nogai", "", "nog", StringArray { "RU" });
		languages.emplace_back ("N'Ko", "", "nqo", StringArray { "GN" });
		languages.emplace_back ("Northern Sotho", "", "nso", StringArray { "ZA" });
		languages.emplace_back ("Nyamwezi", "", "nym");
		languages.emplace_back ("Nyankole", "", "nyn");
		languages.emplace_back ("Nyoro", "", "nyo");
		languages.emplace_back ("Nzima", "", "nzi", StringArray { "GH" });
		languages.emplace_back ("North Ndebele", "nd", "nde", StringArray { "ZW" });
		languages.emplace_back ("Nepali", "ne", "nep", StringArray { "IN", "NP" });
		languages.emplace_back ("Ndonga", "ng", "ndo", StringArray { "NA" });
		languages.emplace_back (CharPointer_UTF8 ("Norwegian Bokmål"), "nb", "nob");
		languages.emplace_back ("Norwegian Nynorsk", "nn", "nno", StringArray { "NO" });
		languages.emplace_back ("Norwegian", "no", "nor", StringArray { "BV", "NO" });
		languages.emplace_back ("Sichuan Yi", "ii", "iii");
		languages.emplace_back ("South Ndebele", "nr", "nbl", StringArray { "ZA" });
		languages.emplace_back ("Occitan", "oc", "oci", StringArray { "MC" });
		languages.emplace_back ("Ojibwa", "oj", "oji");
		languages.emplace_back ("Oromo", "om", "orm", StringArray { "ET" });
		languages.emplace_back ("Osage", "", "osa");
		languages.emplace_back ("Oriya", "or", "ori", StringArray { "IN", "ID" });
		languages.emplace_back ("Ossetian", "os", "oss", StringArray { "RU" });
		languages.emplace_back ("Papuan", "", "paa");
		languages.emplace_back ("Pangasinan", "", "pag", StringArray { "PH" });
		languages.emplace_back ("Pahlavi", "", "pal");
		languages.emplace_back ("Pampanga", "", "pam", StringArray { "PH" });
		languages.emplace_back ("Punjabi", "pa", "pan", StringArray { "IN", "ID", "PK" });
		languages.emplace_back ("Papiamento", "", "pap", StringArray { "NL", "CW", "AW" });
		languages.emplace_back ("Palauan", "", "pau", StringArray { "PW" });
		languages.emplace_back ("Pali", "pi", "pli");
		languages.emplace_back ("Persian", "fa", "per", StringArray { "IR" });
		languages.emplace_back ("Polish", "pl", "pol", StringArray { "BY", "CZ", "PL", "RO", "UA" });
		languages.emplace_back ("Pashto", "ps", "pus", StringArray { "AF", "PK" });
		languages.emplace_back ("Pohnpeian", "", "pon", StringArray { "FM" });
		languages.emplace_back ("Portuguese", "pt", "por",
								StringArray { "AO", "BR", "CV", "CO", "GQ", "GY", "MO", "MZ", "PT", "ST", "ZA", "SR", "TL", "UY", "VE" });
		languages.emplace_back ("Quechua", "qu", "que", StringArray { "AR", "BO", "CO", "EC", "PE" });
		languages.emplace_back ("Rajasthani", "", "raj");
		languages.emplace_back ("Rapanui", "", "rap");
		languages.emplace_back ("Rarotongan", "", "rar");
		languages.emplace_back ("Romani", "", "rom", StringArray { "NO", "RO", "SE", "MX", "MK", "CZ" });
		languages.emplace_back ("Romansh", "rm", "roh", StringArray { "CH" });
		languages.emplace_back ("Rundi", "rn", "run", StringArray { "BI" });
		languages.emplace_back ("Romanian", "ro", "rum", StringArray { "MD", "RO", "UA" });
		languages.emplace_back ("Russian", "ru", "rus", StringArray { "AZ", "BY", "CZ", "KZ", "KG", "MD", "RO", "RU", "TJ", "TM", "UA", "UZ" });
		languages.emplace_back ("Sandawe", "", "sad");
		languages.emplace_back ("Yakut", "", "sah", StringArray { "RU" });
		languages.emplace_back ("Samaritan Aramaic", "", "sam");
		languages.emplace_back ("Sanskrit", "sa", "san", StringArray { "IN", "ZA" });
		languages.emplace_back ("Sasak", "", "sas", StringArray { "ID" });
		languages.emplace_back ("Santali", "", "sat", StringArray { "IN" });
		languages.emplace_back ("Sicilian", "", "scn");
		languages.emplace_back ("Scots", "", "sco", StringArray { "GB" });
		languages.emplace_back ("Selkup", "", "sel");
		languages.emplace_back ("Shan", "", "shn", StringArray { "MM" });
		languages.emplace_back ("Sidamo", "", "sid", StringArray { "ET" });
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Skolt Sami", "", "sms");
		languages.emplace_back ("Sardinian", "sc", "srd", StringArray { "IT" });
		languages.emplace_back ("Sindhi", "sd", "snd", StringArray { "IN", "ID", "PK" });
		languages.emplace_back ("Northern Sami", "se", "sme", StringArray { "FI" });
		languages.emplace_back ("Samoan", "sm", "smo", StringArray { "AS", "WS" });
		languages.emplace_back ("Sango", "sg", "sag", StringArray { "CF" });
		languages.emplace_back ("Serbian", "sr", "srp", StringArray { "BA", "CZ", "MK", "ME", "RO", "RS" });
		languages.emplace_back ("Gaelic", "gd", "gla", StringArray { "GB" });
		languages.emplace_back ("Shona", "sn", "sna", StringArray { "ZW" });
		languages.emplace_back ("Sinhala", "si", "sin", StringArray { "LK" });
		languages.emplace_back ("Soninke", "", "snk", StringArray { "SN", "MR", "GW", "GM" });
		languages.emplace_back ("Sranan Tongo", "", "srn", StringArray { "SR" });
		languages.emplace_back ("Serer", "", "srr", StringArray { "SN", "GW", "GM" });
		languages.emplace_back ("Sukuma", "", "suk", StringArray { "TZ" });
		languages.emplace_back ("Susu", "", "sus", StringArray { "GN", "GQ" });
		languages.emplace_back ("Syriac", "", "syr");
		languages.emplace_back ("Slovak", "sk", "slo", StringArray { "CZ", "RO", "SK", "UA" });
		languages.emplace_back ("Slovenian", "sl", "slv", StringArray { "AT", "SI" });
		languages.emplace_back ("Somali", "so", "som", StringArray { "DJ", "ET", "SO" });
		languages.emplace_back ("Sotho", "st", "sot", StringArray { "LS", "ZA", "ZW" });
		languages.emplace_back ("Spanish", "es", "spa",
								StringArray { "AR", "BZ", "BO", "CL", "CO", "CR", "CU", "DO", "EC", "SV", "GQ", "GI", "GT", "GY", "HN", "MX", "NI", "PA", "PY", "PE", "PH", "PR", "ES", "SR", "TT", "US", "UY", "VE", "EH" });
		languages.emplace_back ("Sundanese", "su", "sun", StringArray { "ID" });
		languages.emplace_back ("Swahili", "sw", "swa", StringArray { "CD", "KE", "MZ", "RW", "TZ", "UG" });
		languages.emplace_back ("Swati", "ss", "ssw", StringArray { "SZ", "ZA" });
		languages.emplace_back ("Swedish", "sv", "swe", StringArray { "AX", "FI", "SE" });
		languages.emplace_back ("Timne", "", "tem", StringArray { "GN" });
		languages.emplace_back ("Tereno", "", "ter");
		languages.emplace_back ("Tetum", "", "tet", StringArray { "TL" });
		languages.emplace_back ("Tigre", "", "tig", StringArray { "ER" });
		languages.emplace_back ("Tiv", "", "tiv", StringArray { "NG" });
		languages.emplace_back ("Tokelau", "", "tkl", StringArray { "TK" });
		languages.emplace_back ("Tlingit", "", "tli");
		languages.emplace_back ("Tamashek", "", "tmh", StringArray { "NE" });
		languages.emplace_back ("Tok Pisin", "", "tpi", StringArray { "PG" });
		languages.emplace_back ("Tsimshian", "", "tsi");
		languages.emplace_back ("Tumbuka", "", "tum", StringArray { "ZM", "MW" });
		languages.emplace_back ("Tuvalu", "", "tvl", StringArray { "TV" });
		languages.emplace_back ("Tuvinian", "", "tyv", StringArray { "RU" });
		languages.emplace_back ("Tamil", "ta", "tam", StringArray { "IN", "ID", "SG", "ZA", "LK" });
		languages.emplace_back ("Telugu", "te", "tel", StringArray { "IN", "ID", "ZA" });
		languages.emplace_back ("Tajik", "tg", "tgk", StringArray { "TJ", "UZ" });
		languages.emplace_back ("Thai", "th", "tha", StringArray { "TH" });
		languages.emplace_back ("Tigrinya", "ti", "tir", StringArray { "ER", "ET" });
		languages.emplace_back ("Tibetan", "bo", "tib", StringArray { "CN" });
		languages.emplace_back ("Turkmen", "tk", "tuk", StringArray { "IR", "KZ", "TJ", "TM", "UZ" });
		languages.emplace_back ("Tagalog", "tl", "tgl", StringArray { "PH" });
		languages.emplace_back ("Tswana", "tn", "tsn", StringArray { "BW", "NA", "ZA", "ZW" });
		languages.emplace_back ("Tonga", "to", "ton", StringArray { "MW", "TO" });
		languages.emplace_back ("Turkish", "tr", "tur", StringArray { "CY", "IQ", "MK", "RO", "RU", "TR" });
		languages.emplace_back ("Tsonga", "ts", "tso", StringArray { "MZ", "ZA", "ZW" });
		languages.emplace_back ("Tatar", "tt", "tat", StringArray { "KZ", "RU", "TJ", "TM" });
		languages.emplace_back ("Twi", "tw", "twi", StringArray { "GH" });
		languages.emplace_back ("Tahitian", "ty", "tah", StringArray { "PF" });
		languages.emplace_back ("Udmurt", "", "udm", StringArray { "RU" });
		languages.emplace_back ("Ugaritic", "", "uga");
		languages.emplace_back ("Umbundu", "", "umb", StringArray { "AO" });
		languages.emplace_back ("Uighur", "ug", "uig", StringArray { "CN", "KZ", "KG", "UZ" });
		languages.emplace_back ("Ukrainian", "uk", "ukr", StringArray { "BY", "CZ", "KZ", "MD", "RO", "RU", "TJ", "TM", "UA", "UZ" });
		languages.emplace_back ("Urdu", "ur", "urd", StringArray { "IN", "PK", "ZA" });
		languages.emplace_back ("Uzbek", "uz", "uzb", StringArray { "KZ", "KG", "TJ", "TM", "UZ" });
		languages.emplace_back ("Vai", "", "vai");
		languages.emplace_back ("Votic", "", "vot");
		languages.emplace_back ("Venda", "ve", "ven", StringArray { "ZA", "ZW" });
		languages.emplace_back ("Vietnamese", "vi", "vie", StringArray { "CZ", "VN" });
		languages.emplace_back (CharPointer_UTF8 ("Volapük"), "vo", "vol");
		languages.emplace_back ("Walloon", "wa", "wln");
		languages.emplace_back ("Welsh", "cy", "wel", StringArray { "AR", "GB" });
		languages.emplace_back ("Wolof", "wo", "wol", StringArray { "GM", "MR", "SN" });
		languages.emplace_back ("Wolaitta", "", "wal", StringArray { "ET" });
		languages.emplace_back ("Washo", "", "was");
		languages.emplace_back ("Kalmyk", "", "xal");
		languages.emplace_back ("Western Frisian", "fy", "fry", StringArray { "NL" });
		languages.emplace_back ("Xhosa", "xh", "xho", StringArray { "ZA", "ZW" });
		languages.emplace_back ("Yao", "", "yao", StringArray { "MW" });
		languages.emplace_back ("Yapese", "", "yap", StringArray { "FM" });
		languages.emplace_back ("Yiddish", "yi", "yid", StringArray { "BY", "RO", "SE", "UA" });
		languages.emplace_back ("Yoruba", "yo", "yor", StringArray { "BJ", "NG" });
		languages.emplace_back ("Zapotec", "", "zap", StringArray { "MX" });
		languages.emplace_back ("Zenaga", "", "zen");
		languages.emplace_back ("Standard Moroccan Tamazight", "", "zgh");
		languages.emplace_back ("Zhuang", "za", "zha");
		languages.emplace_back ("Zulu", "zu", "zul", StringArray { "ZA" });
		languages.emplace_back ("Zuni", "", "zun");
		languages.emplace_back ("Zaza", "", "zza");
	}

	[[nodiscard]] const LanguageData getDataForLanguageName (const String& name) const
	{
		jassert (name.isNotEmpty());

		if (auto res = std::find_if (languages.begin(), languages.end(),
									 [&name] (const LanguageData& lang)
									 { return lang.languageName == name; });
			res != languages.end())
			return *res;

		return {};
	}

	[[nodiscard]] const LanguageData getDataForISO639_1 (const String& code) const
	{
		jassert (code.length() == 2);

		if (auto res = std::find_if (languages.begin(), languages.end(),
									 [&code] (const LanguageData& lang)
									 { return lang.ISO639_1 == code; });
			res != languages.end())
			return *res;

		return {};
	}

	[[nodiscard]] const LanguageData getDataForISO639_2 (const String& code) const
	{
		jassert (code.length() == 3);

		if (auto res = std::find_if (languages.begin(), languages.end(),
									 [&code] (const LanguageData& lang)
									 { return lang.ISO639_2 == code; });
			res != languages.end())
			return *res;

		return {};
	}

	[[nodiscard]] StringArray getLanguageNamesForCountryCode (const String& code) const
	{
		jassert (code.length() == 2);

		StringArray langs;

		for (const auto& lang : languages)
			if (lang.countryCodes.contains (code))
				langs.add (lang.languageName);

		return langs;
	}

	[[nodiscard]] StringArray getLanguageCodesForCountryCode (const String& code, bool asISO639_2) const
	{
		jassert (code.length() == 2);

		StringArray langs;

		for (const auto& lang : languages)
		{
			if (lang.countryCodes.contains (code))
			{
				if (asISO639_2)
					langs.add (lang.ISO639_2);
				else
					langs.add (lang.ISO639_1);
			}
		}

		return langs;
	}

private:

	std::vector<LanguageData> languages;
};


static const KnownLanguages& getLanguages()
{
	static const KnownLanguages languages;

	return languages;
}


String languageNameToCode (const String& name, bool asISO639_2)
{
	const auto data = getLanguages().getDataForLanguageName (name);

	if (! data.isValid())
		return {};

	if (asISO639_2)
		return data.ISO639_2;

	return data.ISO639_1;
}

String languageCodeToName (const String& languageCode)
{
	const auto data = [languageCode]() -> LanguageData
	{
		if (languageCode.length() == 2)
			return getLanguages().getDataForISO639_1 (languageCode);

		if (languageCode.length() == 3)
			return getLanguages().getDataForISO639_2 (languageCode);

		return {};
	}();

	if (! data.isValid())
		return {};

	return data.languageName;
}

StringArray getCountryCodesForLanguage (const String& language, bool languageName)
{
	const auto language_name = [languageName, language]
	{
		if (languageName)
			return language;

		return languageCodeToName (language);
	}();

	const auto data = getLanguages().getDataForLanguageName (language_name);

	if (! data.isValid())
		return {};

	return data.countryCodes;
}

StringArray getLanguageNamesForCountryCode (const String& countryCode)
{
	if (countryCode.length() != 2)
		return {};

	return getLanguages().getLanguageNamesForCountryCode (countryCode);
}

StringArray getLanguageCodesForCountry (const String& countryCode, bool asISO639_2)
{
	if (countryCode.length() != 2)
		return {};

	return getLanguages().getLanguageCodesForCountryCode (countryCode, asISO639_2);
}

}  // namespace lemons::locale
