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

#pragma once

namespace lemons
{

class TextTable final
{
public:

	/** Appends text to the current row.
	 */
	void addColumnToCurrentRow (const String& text);

	/** Appends text to the current row. This is identical to calling addColumnToCurrentRow().
	 */
	TextTable& operator<< (const String& text);

	/** Starts a new row; subsequent calls to addTextToCurrentRow() will be added to the next row.
	 */
	void startNewRow();

	/** Starts a new row. This is identical to calling startNewRow(). */
	TextTable& operator++();

	/** Clears and resets the table. */
	void clear();

	/** Returns the current number of rows in the table. */
	[[nodiscard]] int getNumRows() const noexcept;

	/** Finds the total number of columns, by looking for the row with the greatest number of columns. */
	[[nodiscard]] int getNumColumns() const noexcept;

	/** Returns a StringArray with a string for each row.
		The prefix and suffix will be appended to each row, and and the column separator will be placed between each column.
	 */
	[[nodiscard]] juce::StringArray getRows (const String& rowPrefix,
											 const String& columnSeparator,
											 const String& rowSuffix) const;

	/** Returns a string containing all the rows in the table.
	 */
	[[nodiscard]] String toString (const String& rowPrefix,
								   const String& columnSeparator,
								   const String& rowSuffix) const;

private:

	struct Row final
	{
		[[nodiscard]] String toString (const String&			  columnSeparator,
									   const std::vector<size_t>& widths) const;

		juce::StringArray columns;
	};

	[[nodiscard]] std::vector<size_t> getColumnWidths() const;

	juce::OwnedArray<Row> rows;

	bool newRow { true };
};

}  // namespace lemons
