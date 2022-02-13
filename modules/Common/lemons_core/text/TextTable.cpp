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

namespace lemons
{

String TextTable::Row::toString (const String&				columnSeparator,
								 const std::vector<size_t>& widths) const
{
	String result;

	size_t index = 0;

	for (const auto width : widths)
	{
		if (index != 0)
			result += columnSeparator;

		String padded;

		if (index < columns.size())
			padded = columns[index];

		if (width < padded.length())
			padded = padded.dropLastCharacters (padded.length() - width);
		else
			padded = padded.paddedRight (' ', width);

		result += padded;
		++index;
	}

	return result;
}

/*-----------------------------------------------------------------------------------------------------------------*/

void TextTable::clear()
{
	rows.clear();
	newRow = true;
}

void TextTable::startNewRow()
{
	if (newRow)
		rows.add (new Row);
	else
		newRow = true;
}

TextTable& TextTable::operator++()
{
	startNewRow();
	return *this;
}

void TextTable::addColumnToCurrentRow (const String& text)
{
	if (newRow || rows.isEmpty())
	{
		newRow = false;
		rows.add (new Row);
	}

	rows.getLast()->columns.add (text);
}

TextTable& TextTable::operator<< (const String& text)
{
	addColumnToCurrentRow (text);
	return *this;
}

int TextTable::getNumRows() const noexcept
{
	return rows.size();
}

int TextTable::getNumColumns() const noexcept
{
	int maxColumns = 0;

	for (const auto* row : rows)
		maxColumns = std::max (maxColumns, row->columns.size());

	return maxColumns;
}

juce::StringArray TextTable::getRows (const String& rowPrefix,
									  const String& columnSeparator,
									  const String& rowSuffix) const
{
	juce::StringArray result;

	const auto widths = getColumnWidths();

	for (const auto* row : rows)
		result.add (rowPrefix + row->toString (columnSeparator, widths) + rowSuffix);

	return result;
}

String TextTable::toString (const String& rowPrefix,
							const String& columnSeparator,
							const String& rowSuffix) const
{
	String result;

	for (const auto& row : getRows (rowPrefix, columnSeparator, rowSuffix))
		result += row + juce::newLine;	// cppcheck-suppress useStlAlgorithm

	return result;
}

std::vector<size_t> TextTable::getColumnWidths() const
{
	std::vector<size_t> widths;
	widths.resize (getNumColumns());

	for (auto& row : rows)
		for (size_t i = 0; i < row->columns.size(); ++i)
			widths[i] = std::max (widths[i], static_cast<size_t> (row->columns[i].length()));

	return widths;
}

}  // namespace lemons
