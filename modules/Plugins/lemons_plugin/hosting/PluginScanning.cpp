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

namespace lemons::plugin::scanning
{

juce::AudioPluginFormatManager& getDefaultPluginFormatManager()
{
	struct DefaultPluginFormats final : public juce::AudioPluginFormatManager
	{
		explicit DefaultPluginFormats()
		{
			addDefaultFormats();
		}
	};

	static DefaultPluginFormats manager;

	return manager;
}


std::unique_ptr<juce::KnownPluginList> scanDirectoryForPlugins (juce::FileSearchPath rootDirectory,
																const File&			 blacklistFile)
{
	auto list = std::make_unique<juce::KnownPluginList>();

	for (auto* format : getDefaultPluginFormatManager().getFormats())
	{
		juce::PluginDirectoryScanner scanner { *list, *format, rootDirectory, true, blacklistFile, false };

		while (true)
		{
			String name;

			if (! scanner.scanNextFile (true, name))
				break;
		}
	}

	return list;
}

/*------------------------------------------------------------------------------------------------------------------------*/


Category::Category (const String&			  categoryName,
					SortMethod				  sortMethodToUse,
					SortingCallback			  sortingCallback,
					SubcategoryNamingCallback subcategoryNaming)
	: shouldPutInThisCategory (sortingCallback), getSubcategoryName (subcategoryNaming), sortMethod (sortMethodToUse), name (categoryName)
{
}

Category::SortMethod Category::getSortMethod() const noexcept
{
	return sortMethod;
}

String Category::getName() const
{
	return name;
}

const juce::OwnedArray<Category>& Category::getSubcategories() const
{
	return subcategories;
}

const juce::Array<juce::PluginDescription>& Category::getPlugins() const
{
	return plugins;
}


static inline juce::KnownPluginList::SortMethod getSortingMethod (Category::SortMethod sortMethod)
{
	if (sortMethod == Category::SortMethod::sortByCategory)
		return juce::KnownPluginList::SortMethod::sortByCategory;

	if (sortMethod == Category::SortMethod::sortByManufacturer)
		return juce::KnownPluginList::SortMethod::sortByManufacturer;

	return juce::KnownPluginList::SortMethod::sortByFormat;
}


void Category::addFromPluginTree (const juce::KnownPluginList::PluginTree& tree)
{
	for (const auto& plugin : tree.plugins)
	{
		if (shouldPutInThisCategory (plugin, *this))
		{
			plugins.add (plugin);
			continue;
		}

		auto* subcategory = [this, plugin]
		{
			const auto subcategoryName = getSubcategoryName (plugin);

			for (auto* subcat : subcategories)
				if (subcat->getName() == subcategoryName)
					return subcat;

			return subcategories.add (new Category (subcategoryName, sortMethod,
													shouldPutInThisCategory, getSubcategoryName));
		}();

		jassert (subcategory != nullptr);

		subcategory->plugins.add (plugin);
	}

	for (const auto* subfolder : tree.subFolders)
		addFromPluginTree (*subfolder);
}


static inline void fromCategoryToKnownPluginList (juce::KnownPluginList& list, const Category& category)
{
	for (const auto& plugin : category.getPlugins())
		list.addType (plugin);

	for (const auto* subcat : category.getSubcategories())
		fromCategoryToKnownPluginList (list, *subcat);
}

std::unique_ptr<juce::KnownPluginList> Category::createKnownPluginList() const
{
	auto list = std::make_unique<juce::KnownPluginList>();

	fromCategoryToKnownPluginList (*list, *this);

	return list;
}

Category::SortingCallback Category::getDefaultSortingCallback (SortMethod sortMethod)
{
	if (sortMethod == Category::SortMethod::sortByCategory)
	{
		return [] (const juce::PluginDescription& d, const Category& c)
		{
			auto& plugins = c.getPlugins();

			return ! std::any_of (plugins.begin(), plugins.end(),
								  [&d] (const juce::PluginDescription& plugin)
								  { return d.category != plugin.category; });
		};
	}

	if (sortMethod == Category::SortMethod::sortByManufacturer)
	{
		return [] (const juce::PluginDescription& d, const Category& c)
		{
			auto& plugins = c.getPlugins();

			return ! std::any_of (plugins.begin(), plugins.end(),
								  [&d] (const juce::PluginDescription& plugin)
								  { return d.manufacturerName != plugin.manufacturerName; });
		};
	}

	return [] (const juce::PluginDescription& d, const Category& c)
	{
		auto& plugins = c.getPlugins();

		return ! std::any_of (plugins.begin(), plugins.end(),
							  [&d] (const juce::PluginDescription& plugin)
							  { return d.pluginFormatName != plugin.pluginFormatName; });
	};
}


Category::SubcategoryNamingCallback Category::getDefaultSubcategoryNamingCallback (SortMethod sortMethod)
{
	if (sortMethod == Category::SortMethod::sortByCategory)
	{
		return [] (const juce::PluginDescription& d)
		{ return d.category; };
	}

	if (sortMethod == Category::SortMethod::sortByManufacturer)
	{
		return [] (const juce::PluginDescription& d)
		{ return d.manufacturerName; };
	}

	return [] (const juce::PluginDescription& d)
	{ return d.pluginFormatName; };
}


std::unique_ptr<Category> scanDirectory (juce::FileSearchPath rootDirectory,
										 const File&		  blacklistFile,
										 Category::SortMethod sortMethod,
										 const String&		  rootCategoryName)
{
	auto category = std::make_unique<Category> (rootCategoryName, sortMethod,
												Category::getDefaultSortingCallback (sortMethod),
												Category::getDefaultSubcategoryNamingCallback (sortMethod));

	const auto list = scanDirectoryForPlugins (rootDirectory, blacklistFile);
	const auto tree = juce::KnownPluginList::createTree (list->getTypes(), getSortingMethod (sortMethod));

	category->addFromPluginTree (*tree);

	return category;
}

}  // namespace lemons::plugin::scanning


namespace juce
{

PluginDescription VariantConverter<PluginDescription>::fromVar (const var& v)
{
	PluginDescription d;

	d.loadFromXml (VariantConverter<XmlElement>::fromVar (v));

	return d;
}

var VariantConverter<PluginDescription>::toVar (const PluginDescription& d)
{
	return VariantConverter<XmlElement>::toVar (*d.createXml());
}

}  // namespace juce
