#pragma once

namespace lemons::plugin::scanning
{

[[nodiscard]] juce::AudioPluginFormatManager& getDefaultPluginFormatManager();


[[nodiscard]] std::unique_ptr<juce::KnownPluginList> scanDirectoryForPlugins (juce::FileSearchPath rootDirectory,
																			  const File&		   blacklistFile);


struct Category
{
	enum class SortMethod
	{
		sortByCategory,
		sortByManufacturer,
		sortByFormat
	};

	using SortingCallback = std::function<bool (const juce::PluginDescription&, const Category&)>;

	using SubcategoryNamingCallback = std::function<String (const juce::PluginDescription&)>;

	explicit Category (const String&			 categoryName,
					   SortMethod				 sortMethodToUse,
					   SortingCallback			 sortingCallback,
					   SubcategoryNamingCallback subcategoryNaming);

	void addFromPluginTree (const juce::KnownPluginList::PluginTree& tree);

	[[nodiscard]] SortMethod getSortMethod() const noexcept;

	[[nodiscard]] String getName() const;

	[[nodiscard]] const juce::OwnedArray<Category>& getSubcategories() const;

	[[nodiscard]] const juce::Array<juce::PluginDescription>& getPlugins() const;

	[[nodiscard]] std::unique_ptr<juce::KnownPluginList> createKnownPluginList() const;

	[[nodiscard]] static SortingCallback getDefaultSortingCallback (SortMethod method);

	[[nodiscard]] static SubcategoryNamingCallback getDefaultSubcategoryNamingCallback (SortMethod method);

private:

	SortingCallback shouldPutInThisCategory;

	SubcategoryNamingCallback getSubcategoryName;

	SortMethod sortMethod;

	String name;

	juce::Array<juce::PluginDescription> plugins;

	juce::OwnedArray<Category> subcategories;
};


[[nodiscard]] std::unique_ptr<Category> scanDirectory (juce::FileSearchPath rootDirectory,
													   const File&			blacklistFile,
													   Category::SortMethod sortMethod,
													   const String&		rootCategoryName = "Plugins");

}  // namespace lemons::plugin::scanning


namespace juce
{

template <>
struct VariantConverter<PluginDescription>
{
	static PluginDescription fromVar (const var& v);
	static var				 toVar (const PluginDescription& d);
};

}  // namespace juce
