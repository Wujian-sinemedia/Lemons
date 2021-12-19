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

using juce::StringArray;

struct LanguageData final
{
	LanguageData()
	    : languageName ("Unknown")
	{
	}

	LanguageData (const String& name,
	              const String& ISO_639_1, const String& ISO_639_2,
	              const StringArray& countries = {})
	    : languageName (name)
	    , ISO639_1 (ISO_639_1)
	    , ISO639_2 (ISO_639_2)
	    , countryCodes (countries)
	{
		jassert (ISO639_1.isEmpty() || ISO639_1.length() == 2);
		jassert (ISO639_2.length() == 3);
	}

	[[nodiscard]] bool isValid() const
	{
		return languageName != "Unknown" && (ISO639_1.isNotEmpty() || ISO639_2.isNotEmpty());
	}

	String languageName, ISO639_1, ISO639_2;

	StringArray countryCodes;
};


/** Note that ISO639-2/B codes are used instead of ISO639-2/T codes.
    See https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
    And https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes

 JUST DID MYANMAR
 */
struct KnownLanguages final
{
	KnownLanguages()
	{
		using juce::CharPointer_UTF8;

		languages.emplace_back ("Abkhazian", "ab", "abk", StringArray { "GE" });
		languages.emplace_back ("Achinese", "", "ace");
		languages.emplace_back ("Acoli", "", "ach");
		languages.emplace_back ("Adangme", "", "ada");
		languages.emplace_back ("Adyghe", "", "ady");
		languages.emplace_back ("Afar", "aa", "aar", StringArray { "DJ", "ER", "ET" });
		languages.emplace_back ("Afrihili", "", "afh");
		languages.emplace_back ("Afrikaans", "af", "afr");
		languages.emplace_back ("Ainu", "", "ain");
		languages.emplace_back ("Akan", "ak", "aka");
		languages.emplace_back ("Albanian", "sq", "alb", StringArray { "AL", "IT", "MK", "ME" });
		languages.emplace_back ("Aleut", "", "ale");
		languages.emplace_back ("Amharic", "am", "amh", StringArray { "ET" });
		languages.emplace_back ("Angika", "", "anp");
		languages.emplace_back ("Arabic", "ar", "ara",
		                        StringArray { "DZ", "BH", "BJ", "TD", "KM", "DJ", "EG", "ER", "GM", "GN", "GW", "ID", "IR", "IL", "JO", "KW", "LB", "LY", "ML", "MR", "MA" });
		languages.emplace_back ("Aragonese", "an", "arg");
		languages.emplace_back ("Arapaho", "", "arp");
		languages.emplace_back ("Arawak", "", "arw");
		languages.emplace_back ("Armenian", "hy", "arm", StringArray { "AM", "CY", "IQ" });
		languages.emplace_back ("Aromanian", "", "rup");
		languages.emplace_back ("Assamese", "as", "asm", StringArray { "IN" });
		languages.emplace_back ("Avaric", "av", "ava");
		languages.emplace_back ("Avestan", "ae", "ave");
		languages.emplace_back ("Awadhi", "", "awa");
		languages.emplace_back ("Aymara", "ay", "aym", StringArray { "BO" });
		languages.emplace_back ("Azerbaijani", "az", "aze", StringArray { "AZ", "IR", "KZ" });
		languages.emplace_back ("Balinese", "", "ban");
		languages.emplace_back ("Baluchi", "", "bal");
		languages.emplace_back ("Bambara", "bm", "bam");
		languages.emplace_back ("Basa", "", "bas");
		languages.emplace_back ("Bashkir", "ba", "bak");
		languages.emplace_back ("Basque", "eu", "baq");
		languages.emplace_back ("Beja", "", "bej");
		languages.emplace_back ("Belarusian", "be", "bel", StringArray { "BY", "CZ" });
		languages.emplace_back ("Bemba", "", "bem");
		languages.emplace_back ("Bengali", "bn", "ben", StringArray { "BD", "IN", "ID" });
		languages.emplace_back ("Bhojpuri", "", "bho");
		languages.emplace_back ("Bikol", "", "bik");
		languages.emplace_back ("Bini", "", "bin");
		languages.emplace_back ("Bislama", "bi", "bis");
		languages.emplace_back ("Bilin", "", "byn");
		languages.emplace_back ("Bokmål", "", "nob");
		languages.emplace_back ("Bosnian", "bs", "bos", StringArray { "BA", "MK", "ME" });
		languages.emplace_back ("Braj", "", "bra");
		languages.emplace_back ("Breton", "br", "bre");
		languages.emplace_back ("Buginese", "", "bug");
		languages.emplace_back ("Bulgarian", "bg", "bul", StringArray { "BG" });
		languages.emplace_back ("Buriat", "", "bua");
		languages.emplace_back ("Burmese", "my", "bur", StringArray { "MM" });
		languages.emplace_back ("Caddo", "", "cad");
		languages.emplace_back ("Catalan", "ca", "cat", StringArray { "AD", "IT" });
		languages.emplace_back ("Cebuano", "", "ceb");
		languages.emplace_back ("Central Khmer", "", "khm");
		languages.emplace_back ("Chagatai", "", "chg");
		languages.emplace_back ("Chamorro", "ch", "cha", StringArray { "GU" });
		languages.emplace_back ("Chechen", "ce", "che");
		languages.emplace_back ("Cherokee", "", "chr");
		languages.emplace_back ("Cheyenne", "", "chy");
		languages.emplace_back ("Chibcha", "", "chb");
		languages.emplace_back ("Chichewa", "ny", "nya", StringArray { "MW", "MZ" });
		languages.emplace_back ("Chinese", "zh", "chi", StringArray { "BZ", "BN", "CN", "GY", "HK", "MO" });
		languages.emplace_back ("Chinook jargon", "", "chn");
		languages.emplace_back ("Chipewyan", "", "chp");
		languages.emplace_back ("Choctaw", "", "cho");
		languages.emplace_back ("Chuukese", "", "chk");
		languages.emplace_back ("Chuvash", "cv", "chv");
		languages.emplace_back ("Cornish", "kw", "cor");
		languages.emplace_back ("Corsican", "co", "cos");
		languages.emplace_back ("Cree", "cr", "cre");
		languages.emplace_back ("Creek", "", "mus");
		languages.emplace_back ("Crimean Tatar", "", "crh");
		languages.emplace_back ("Croatian", "hr", "hrv", StringArray { "AT", "BA", "HR", "ME" });
		languages.emplace_back ("Czech", "cs", "cze", StringArray { "CZ" });
		languages.emplace_back ("Dakota", "", "dak");
		languages.emplace_back ("Danish", "da", "dan", StringArray { "DK", "FO" });
		languages.emplace_back ("Dargwa", "", "dar");
		languages.emplace_back ("Delaware", "", "del");
		languages.emplace_back ("Dinka", "", "din");
		languages.emplace_back ("Divehi", "dv", "div", StringArray { "MV" });
		languages.emplace_back ("Dogri", "", "doi");
		languages.emplace_back ("Dogrib", "", "dgr");
		languages.emplace_back ("Duala", "", "dua");
		languages.emplace_back ("Dutch", "nl", "dut", StringArray { "AW", "BE", "BQ", "CW", "GY", "ID" });
		languages.emplace_back ("Dyula", "", "dyu");
		languages.emplace_back ("Dzongkha", "dz", "dzo", StringArray { "BT" });
		languages.emplace_back ("East Frisian Low Saxon", "", "frs");
		languages.emplace_back ("Efik", "", "efi");
		languages.emplace_back ("Ekajuk", "", "eka");
		languages.emplace_back ("English", "en", "eng",
		                        StringArray { "AS", "AI", "AG", "AU", "AZ", "BS", "BH", "BB", "BZ", "BJ", "BM", "BQ", "BW", "IO", "BN", "BI", "CM", "CA", "KY", "CX", "CC", "CO", "CK", "CU", "CW", "DM", "ER", "SZ", "FK", "FJ", "FI", "GM", "GH", "GI", "GD", "GU", "GG", "GW", "GY", "HK", "IN", "ID", "IQ", "IE", "JM", "KE", "KI", "LS", "LR", "MW", "MY", "MV", "MT", "MH", "MU", "FM" });
		languages.emplace_back ("Erzya", "", "myv");
		languages.emplace_back ("Esperanto", "eo", "epo");
		languages.emplace_back ("Estonian", "et", "est", StringArray { "EE" });
		languages.emplace_back ("Ewe", "ee", "ewe", StringArray { "GH" });
		languages.emplace_back ("Ewondo", "", "ewo");
		languages.emplace_back ("Fang", "", "fan");
		languages.emplace_back ("Fanti", "", "fat");
		languages.emplace_back ("Faroese", "fo", "fao", StringArray { "FO" });
		languages.emplace_back ("Fijian", "fj", "fij", StringArray { "FJ" });
		languages.emplace_back ("Filipino", "", "fil");
		languages.emplace_back ("Finnish", "fi", "fin", StringArray { "FI" });
		languages.emplace_back ("Fon", "", "fon");
		languages.emplace_back ("French", "fr", "fre",
		                        StringArray { "DZ", "BE", "BJ", "BF", "BI", "CM", "CA", "CF", "TD", "KM", "CD", "CG", "CI", "DJ", "GQ", "FR", "PF", "GM", "GN", "GY", "HT", "VA", "LA", "LB", "LU", "MG", "ML", "MR", "MU", "MC", "MA" });
		languages.emplace_back ("Friulian", "", "fur");
		languages.emplace_back ("Fulah", "ff", "ful", StringArray { "BJ", "CM", "GM", "GN", "ML" });
		languages.emplace_back ("Ga", "", "gaa");
		languages.emplace_back ("Galibi Carib", "", "car");
		languages.emplace_back ("Galician", "gl", "glg");
		languages.emplace_back ("Ganda", "", "lug");
		languages.emplace_back ("Gayo", "", "gay");
		languages.emplace_back ("Gbaya", "", "gba");
		languages.emplace_back ("Georgian", "ka", "geo", StringArray { "GE" });
		languages.emplace_back ("German", "de", "ger", StringArray { "AT", "BE", "BZ", "CR", "CZ", "DK", "DE", "LI", "LU" });
		languages.emplace_back ("Gilbertese", "", "gil");
		languages.emplace_back ("Gondi", "", "gon");
		languages.emplace_back ("Gorontalo", "", "gor");
		languages.emplace_back ("Grebo", "", "grb");
		languages.emplace_back ("Greek", "el", "gre", StringArray { "AL", "CY", "CZ", "GR" });
		languages.emplace_back ("Guarani", "gn", "grn", StringArray { "AR", "BO" });
		languages.emplace_back ("Gujarati", "gu", "guj", StringArray { "IN", "ID" });
		languages.emplace_back ("Gwich'in", "", "gwi");
		languages.emplace_back ("Haida", "", "hai");
		languages.emplace_back ("Haitian", "ht", "hat", StringArray { "CU", "HT" });
		languages.emplace_back ("Hausa", "ha", "hau", StringArray { "BJ" });
		languages.emplace_back ("Hawaiian", "", "haw");
		languages.emplace_back ("Hebrew", "he", "heb", StringArray { "IL" });
		languages.emplace_back ("Herero", "hz", "her");
		languages.emplace_back ("Hiligaynon", "", "hil");
		languages.emplace_back ("Hindi", "hi", "hin", StringArray { "IN" });
		languages.emplace_back ("Hiri Motu", "ho", "hmo");
		languages.emplace_back ("Hmong", "", "hmn");
		languages.emplace_back ("Hungarian", "hu", "hun", StringArray { "AT", "CZ", "HU" });
		languages.emplace_back ("Hupa", "", "hup");
		languages.emplace_back ("Iban", "", "iba");
		languages.emplace_back ("Iloko", "", "ilo");
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Ingush", "", "inh");
		languages.emplace_back ("Interlingua", "ia", "ina");
		languages.emplace_back ("Indonesian", "id", "ind", StringArray { "ID" });
		languages.emplace_back ("Interlingue", "ie", "ile");
		languages.emplace_back ("Irish", "ga", "gle", StringArray { "IE" });
		languages.emplace_back ("Igbo", "ig", "ibo");
		languages.emplace_back ("Inupiaq", "ik", "ipk");
		languages.emplace_back ("Ido", "io", "ido");
		languages.emplace_back ("Icelandic", "is", "ice", StringArray { "IS" });
		languages.emplace_back ("Italian", "it", "ita", StringArray { "CR", "VA", "IT", "MT", "MC" });
		languages.emplace_back ("Inuktitut", "iu", "iku");
		languages.emplace_back ("Inupiaq", "", "ipk");
		languages.emplace_back ("Japanese", "ja", "jpn", StringArray { "ID", "JP" });
		languages.emplace_back ("Javanese", "jv", "jav");
		languages.emplace_back ("Judeo-Arabic", "", "jrb");
		languages.emplace_back ("Judeo-Persian", "", "jpr");
		languages.emplace_back ("Kabardian", "", "kbd");
		languages.emplace_back ("Kabyle", "", "kab");
		languages.emplace_back ("Kachin", "", "kac");
		languages.emplace_back ("Kalaallisut", "kl", "kal", StringArray { "GL" });
		languages.emplace_back ("Kalmyk", "", "xal");
		languages.emplace_back ("Kamba", "", "kam");
		languages.emplace_back ("Kannada", "kn", "kan", StringArray { "IN" });
		languages.emplace_back ("Kanuri", "kr", "kau");
		languages.emplace_back ("Kara-Kalpak", "", "kaa");
		languages.emplace_back ("Karachay-Balkar", "", "krc");
		languages.emplace_back ("Karelian", "", "krl");
		languages.emplace_back ("Kashmiri", "ks", "kas", StringArray { "IN" });
		languages.emplace_back ("Kashubian", "", "csb");
		languages.emplace_back ("Kawi", "", "kaw");
		languages.emplace_back ("Kazakh", "kk", "kaz", StringArray { "KZ" });
		languages.emplace_back ("Khasi", "", "kha");
		languages.emplace_back ("Kikuyu", "", "kik");
		languages.emplace_back ("Kimbundu", "", "kmb");
		languages.emplace_back ("Kinyarwanda", "", "kin");
		languages.emplace_back ("Central Khmer", "km", "khm", StringArray { "KH" });
		languages.emplace_back ("Kikuyu", "ki", "kik");
		languages.emplace_back ("Kinyarwanda", "rw", "kin");
		languages.emplace_back ("Kirghiz", "ky", "kir", StringArray { "KG" });
		languages.emplace_back ("Komi", "kv", "kom");
		languages.emplace_back ("Kongo", "kg", "kon", StringArray { "AO" });
		languages.emplace_back ("Konkani", "", "kok");
		languages.emplace_back ("Korean", "ko", "kor", StringArray { "ID", "KP", "KR" });
		languages.emplace_back ("Kosraean", "", "kos");
		languages.emplace_back ("Kpelle", "", "kpe");
		languages.emplace_back ("Kumyk", "", "kum");
		languages.emplace_back ("Kurdish", "ku", "kur", StringArray { "IR" });
		languages.emplace_back ("Kurukh", "", "kru");
		languages.emplace_back ("Kutenai", "", "kut");
		languages.emplace_back ("Kuanyama", "kj", "kua");
		languages.emplace_back ("Luxembourgish", "lb", "ltz", StringArray { "LU" });
		languages.emplace_back ("Ganda", "lg", "lug");
		languages.emplace_back ("Ladino", "", "lad");
		languages.emplace_back ("Lahnda", "", "lah");
		languages.emplace_back ("Lamba", "", "lam");
		languages.emplace_back ("Limburgan", "li", "lim");
		languages.emplace_back ("Lingala", "ln", "lin", StringArray { "CD", "CG" });
		languages.emplace_back ("Lao", "lo", "lao", StringArray { "LA" });
		languages.emplace_back ("Lithuanian", "lt", "lit", StringArray { "LT" });
		languages.emplace_back ("Luba-Katanga", "lu", "lub");
		languages.emplace_back ("Luba-Lulua", "", "lua");
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Lunda", "", "lun");
		languages.emplace_back ("Luo", "", "luo");
		languages.emplace_back ("Lushai", "", "lus");
		languages.emplace_back ("Latvian", "lv", "lav", StringArray { "LV" });
		languages.emplace_back ("Lezghian", "", "lez");
		languages.emplace_back ("Lojban", "", "jbo");
		languages.emplace_back ("Low German", "", "nds");
		languages.emplace_back ("Lower Sorbian", "", "dsb");
		languages.emplace_back ("Lozi", "", "loz");
		languages.emplace_back ("Madurese", "", "mad");
		languages.emplace_back ("Magahi", "", "mag");
		languages.emplace_back ("Maithili", "", "mai");
		languages.emplace_back ("Makasar", "", "mak");
		languages.emplace_back ("Mandingo", "", "man");
		languages.emplace_back ("Masai", "", "mas");
		languages.emplace_back ("Moksha", "", "mdf");
		languages.emplace_back ("Mandar", "", "mdr");
		languages.emplace_back ("Mende", "", "men");
		languages.emplace_back ("Mi'kmaq", "", "mic");
		languages.emplace_back ("Minangkabau", "", "min");
		languages.emplace_back ("Manchu", "", "mnc");
		languages.emplace_back ("Manipuri", "", "mni");
		languages.emplace_back ("Mohawk", "", "moh");
		languages.emplace_back ("Mossi", "", "mos");
		languages.emplace_back ("Mirandese", "", "mwl");
		languages.emplace_back ("Marwari", "", "mwr");
		languages.emplace_back ("Mayan", "", "myn");
		languages.emplace_back ("Erzya", "", "myv");
		languages.emplace_back ("Nahuatl", "", "nah");
		languages.emplace_back ("Manx", "gv", "glv", StringArray { "IM" });
		languages.emplace_back ("Macedonian", "mk", "mac", StringArray { "AL", "MK" });
		languages.emplace_back ("Malagasy", "mg", "mlg", StringArray { "MG" });
		languages.emplace_back ("Malay", "ms", "may", StringArray { "BN", "CX", "CC", "ID", "MY" });
		languages.emplace_back ("Malayalam", "ml", "mal");
		languages.emplace_back ("Maltese", "mt", "mlt", StringArray { "MT" });
		languages.emplace_back ("Maori", "mi", "mao");
		languages.emplace_back ("Marathi", "mr", "mar", StringArray { "IN" });
		languages.emplace_back ("Marshallese", "mh", "mah", StringArray { "MH" });
		languages.emplace_back ("Mongolian", "mn", "mon", StringArray { "CN", "MN" });
		languages.emplace_back ("Nauru", "na", "nau");
		languages.emplace_back ("Navajo", "nv", "nav", StringArray { "US" });
		languages.emplace_back ("Neapolitan", "", "nap");
		languages.emplace_back ("Newari", "", "new");
		languages.emplace_back ("Nias", "", "nia");
		languages.emplace_back ("Niuean", "", "niu");
		languages.emplace_back ("Nogai", "", "nog");
		languages.emplace_back ("N'Ko", "", "nqo");
		languages.emplace_back ("Northern Sotho", "", "nso");
		languages.emplace_back ("Nyamwezi", "", "nym");
		languages.emplace_back ("Nyankole", "", "nyn");
		languages.emplace_back ("Nyoro", "", "nyo");
		languages.emplace_back ("Nzima", "", "nzi");
		languages.emplace_back ("North Ndebele", "nd", "nde");
		languages.emplace_back ("Nepali", "ne", "nep", StringArray { "IN" });
		languages.emplace_back ("Ndonga", "ng", "ndo");
		languages.emplace_back (CharPointer_UTF8 ("Norwegian Bokmål"), "nb", "nob");
		languages.emplace_back ("Norwegian Nynorsk", "nn", "nno");
		languages.emplace_back ("Norwegian", "no", "nor", StringArray { "BV" });
		languages.emplace_back ("Sichuan Yi", "ii", "iii");
		languages.emplace_back ("South Ndebele", "nr", "nbl");
		languages.emplace_back ("Occitan", "oc", "oci", StringArray { "MC" });
		languages.emplace_back ("Ojibwa", "oj", "oji");
		languages.emplace_back ("Oromo", "om", "orm", StringArray { "ET" });
		languages.emplace_back ("Osage", "", "osa");
		languages.emplace_back ("Oriya", "or", "ori", StringArray { "IN", "ID" });
		languages.emplace_back ("Ossetian", "os", "oss");
		languages.emplace_back ("Papuan", "", "paa");
		languages.emplace_back ("Pangasinan", "", "pag");
		languages.emplace_back ("Pahlavi", "", "pal");
		languages.emplace_back ("Pampanga", "", "pam");
		languages.emplace_back ("Punjabi", "pa", "pan", StringArray { "IN", "ID" });
		languages.emplace_back ("Papiamento", "", "pap");
		languages.emplace_back ("Palauan", "", "pau");
		languages.emplace_back ("Pali", "pi", "pli");
		languages.emplace_back ("Persian", "fa", "per", StringArray { "IR" });
		languages.emplace_back ("Polish", "pl", "pol", StringArray { "BY", "CZ" });
		languages.emplace_back ("Pashto", "ps", "pus", StringArray { "AF" });
		languages.emplace_back ("Pohnpeian", "", "pon");
		languages.emplace_back ("Portuguese", "pt", "por", StringArray { "AO", "BR", "CV", "CO", "GQ", "GY", "MO", "MZ" });
		languages.emplace_back ("Quechua", "qu", "que", StringArray { "AR", "BO", "CO", "EC" });
		languages.emplace_back ("Rajasthani", "", "raj");
		languages.emplace_back ("Rapanui", "", "rap");
		languages.emplace_back ("Rarotongan", "", "rar");
		languages.emplace_back ("Romany", "", "rom");
		languages.emplace_back ("Romansh", "rm", "roh");
		languages.emplace_back ("Rundi", "rn", "run", StringArray { "BI" });
		languages.emplace_back ("Romanian", "ro", "rum", StringArray { "MD" });
		languages.emplace_back ("Russian", "ru", "rus", StringArray { "AZ", "BY", "CZ", "KZ", "KG", "MD" });
		languages.emplace_back ("Sandawe", "", "sad");
		languages.emplace_back ("Yakut", "", "sah");
		languages.emplace_back ("Samaritan Aramaic", "", "sam");
		languages.emplace_back ("Sanskrit", "sa", "san", StringArray { "IN" });
		languages.emplace_back ("Sasak", "", "sas");
		languages.emplace_back ("Santali", "", "sat");
		languages.emplace_back ("Sicilian", "", "scn");
		languages.emplace_back ("Scots", "", "sco");
		languages.emplace_back ("Selkup", "", "sel");
		languages.emplace_back ("Shan", "", "shn");
		languages.emplace_back ("Sidamo", "", "sid");
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Skolt Sami", "", "sms");
		languages.emplace_back ("Sardinian", "sc", "srd", StringArray { "IT" });
		languages.emplace_back ("Sindhi", "sd", "snd", StringArray { "IN", "ID" });
		languages.emplace_back ("Northern Sami", "se", "sme", StringArray { "FI" });
		languages.emplace_back ("Samoan", "sm", "smo", StringArray { "AS" });
		languages.emplace_back ("Sango", "sg", "sag", StringArray { "CF" });
		languages.emplace_back ("Serbian", "sr", "srp", StringArray { "BA", "CZ", "MK", "ME" });
		languages.emplace_back ("Gaelic", "gd", "gla");
		languages.emplace_back ("Shona", "sn", "sna");
		languages.emplace_back ("Sinhala", "si", "sin");
		languages.emplace_back ("Soninke", "", "snk");
		languages.emplace_back ("Sranan Tongo", "", "srn");
		languages.emplace_back ("Serer", "", "srr");
		languages.emplace_back ("Sukuma", "", "suk");
		languages.emplace_back ("Susu", "", "sus");
		languages.emplace_back ("Syriac", "", "syr");
		languages.emplace_back ("Slovak", "sk", "slo", StringArray { "CZ" });
		languages.emplace_back ("Slovenian", "sl", "slv", StringArray { "AT" });
		languages.emplace_back ("Somali", "so", "som", StringArray { "DJ", "ET" });
		languages.emplace_back ("Southern Sotho", "st", "sot", StringArray { "LS" });
		languages.emplace_back ("Spanish", "es", "spa",
		                        StringArray { "AR", "BZ", "BO", "CL", "CO", "CR", "CU", "DO", "EC", "SV", "GQ", "GI", "GT", "GY", "HN", "MX" });
		languages.emplace_back ("Sundanese", "su", "sun", StringArray { "ID" });
		languages.emplace_back ("Swahili", "sw", "swa", StringArray { "CD", "KE", "MZ" });
		languages.emplace_back ("Swati", "ss", "ssw", StringArray { "SZ" });
		languages.emplace_back ("Swedish", "sv", "swe", StringArray { "AX", "FI" });
		languages.emplace_back ("Timne", "", "tem");
		languages.emplace_back ("Tereno", "", "ter");
		languages.emplace_back ("Tetum", "", "tet");
		languages.emplace_back ("Tigre", "", "tig");
		languages.emplace_back ("Tiv", "", "tiv");
		languages.emplace_back ("Tokelau", "", "tkl");
		languages.emplace_back ("Tlingit", "", "tli");
		languages.emplace_back ("Tamashek", "", "tmh");
		languages.emplace_back ("Tok Pisin", "", "tpi");
		languages.emplace_back ("Tsimshian", "", "tsi");
		languages.emplace_back ("Tumbuka", "", "tum");
		languages.emplace_back ("Tuvalu", "", "tvl");
		languages.emplace_back ("Tuvinian", "", "tyv");
		languages.emplace_back ("Tamil", "ta", "tam", StringArray { "IN", "ID" });
		languages.emplace_back ("Telugu", "te", "tel", StringArray { "IN", "ID" });
		languages.emplace_back ("Tajik", "tg", "tgk");
		languages.emplace_back ("Thai", "th", "tha");
		languages.emplace_back ("Tigrinya", "ti", "tir", StringArray { "ER", "ET" });
		languages.emplace_back ("Tibetan", "bo", "tib", StringArray { "CN" });
		languages.emplace_back ("Turkmen", "tk", "tuk", StringArray { "IR", "KZ" });
		languages.emplace_back ("Tagalog", "tl", "tgl");
		languages.emplace_back ("Tswana", "tn", "tsn", StringArray { "BW" });
		languages.emplace_back ("Tonga", "to", "ton", StringArray { "MW" });
		languages.emplace_back ("Turkish", "tr", "tur", StringArray { "CY", "IQ", "MK" });
		languages.emplace_back ("Tsonga", "ts", "tso", StringArray { "MZ" });
		languages.emplace_back ("Tatar", "tt", "tat", StringArray { "KZ" });
		languages.emplace_back ("Twi", "tw", "twi", StringArray { "GH" });
		languages.emplace_back ("Tahitian", "ty", "tah", StringArray { "PF" });
		languages.emplace_back ("Udmurt", "", "udm");
		languages.emplace_back ("Ugaritic", "", "uga");
		languages.emplace_back ("Umbundu", "", "umb");
		languages.emplace_back ("Uighur", "ug", "uig", StringArray { "CN", "KZ", "KG" });
		languages.emplace_back ("Ukrainian", "uk", "ukr", StringArray { "BY", "CZ", "KZ", "MD" });
		languages.emplace_back ("Urdu", "ur", "urd", StringArray { "IN" });
		languages.emplace_back ("Uzbek", "uz", "uzb", StringArray { "KZ", "KG" });
		languages.emplace_back ("Vai", "", "vai");
		languages.emplace_back ("Votic", "", "vot");
		languages.emplace_back ("Venda", "ve", "ven");
		languages.emplace_back ("Vietnamese", "vi", "vie", StringArray { "CZ" });
		languages.emplace_back (CharPointer_UTF8 ("Volapük"), "vo", "vol");
		languages.emplace_back ("Walloon", "wa", "wln");
		languages.emplace_back ("Welsh", "cy", "wel", StringArray { "AR" });
		languages.emplace_back ("Wolof", "wo", "wol", StringArray { "GM", "MR" });
		languages.emplace_back ("Wolaitta", "", "wal");
		languages.emplace_back ("Washo", "", "was");
		languages.emplace_back ("Kalmyk", "", "xal");
		languages.emplace_back ("Wolaitta", "", "wal");
		languages.emplace_back ("Western Frisian", "fy", "fry");
		languages.emplace_back ("Xhosa", "xh", "xho");
		languages.emplace_back ("Yao", "", "yao");
		languages.emplace_back ("Yapese", "", "yap");
		languages.emplace_back ("Yiddish", "yi", "yid", StringArray { "BY" });
		languages.emplace_back ("Yoruba", "yo", "yor", StringArray { "BJ" });
		languages.emplace_back ("Zapotec", "", "zap");
		languages.emplace_back ("Zenaga", "", "zen");
		languages.emplace_back ("Standard Moroccan Tamazight", "", "zgh");
		languages.emplace_back ("Zhuang", "za", "zha");
		languages.emplace_back ("Zulu", "zu", "zul");
		languages.emplace_back ("Zuni", "", "zun");
		languages.emplace_back ("Zaza", "", "zza");
	}

	[[nodiscard]] const LanguageData getDataForLanguageName (const String& name) const
	{
		jassert (name.isNotEmpty());

		for (const auto& lang : languages)
			if (lang.languageName == name)
				return lang;

		return {};
	}

	[[nodiscard]] const LanguageData getDataForISO639_1 (const String& code) const
	{
		jassert (code.length() == 2);

		for (const auto& lang : languages)
			if (lang.ISO639_1 == code)
				return lang;

		return {};
	}

	[[nodiscard]] const LanguageData getDataForISO639_2 (const String& code) const
	{
		jassert (code.length() == 3);

		for (const auto& lang : languages)
			if (lang.ISO639_2 == code)
				return lang;

		return {};
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

String languageCodeToName (const String& countryCode)
{
	const auto data = [&]() -> LanguageData
	{
		if (countryCode.length() == 2)
			return getLanguages().getDataForISO639_1 (countryCode);

		if (countryCode.length() == 3)
			return getLanguages().getDataForISO639_2 (countryCode);

		return {};
	}();

	if (! data.isValid())
		return {};

	return data.languageName;
}

StringArray getCountryCodesForLanguage (const String& languageName)
{
	const auto data = getLanguages().getDataForLanguageName (languageName);

	if (! data.isValid())
		return {};

	return data.countryCodes;
}

}  // namespace lemons::locale
