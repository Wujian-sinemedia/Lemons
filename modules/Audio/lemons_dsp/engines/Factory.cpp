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

namespace lemons::dsp::factory
{

template <template <typename SampleType> typename EngineType>
String getTypeID()
{
	return "";
}


template <typename SampleType>
std::map<String, CreationFunc<SampleType>>& getEngineBank()
{
	static std::map<String, CreationFunc<SampleType>> engines;

	return engines;
}

template std::map<String, CreationFunc<float>>&	 getEngineBank();
template std::map<String, CreationFunc<double>>& getEngineBank();


template <typename SampleType>
void registerEngine (CreationFunc<SampleType> func, const String& typeID)
{
	auto& engines = getEngineBank<SampleType>();

	if (engines.find (typeID) != engines.end())
	{
		DBG ("Duplicate engine TypeID: " << typeID);
		return;
	}

	engines.emplace (std::make_pair (typeID, func));
}

template void registerEngine (CreationFunc<float>, const String&);
template void registerEngine (CreationFunc<double>, const String&);


EngineCreator::EngineCreator (const CreationFunc<float>&  floatFunc,
							  const CreationFunc<double>& doubleFunc,
							  const String&				  typeID)
{
	registerEngine<float> (floatFunc, typeID);
	registerEngine<double> (doubleFunc, typeID);
}


template <typename SampleType>
[[nodiscard]] static CreationFunc<SampleType> findCreationFunction (const String& typeID)
{
	return getEngineBank<SampleType>().at (typeID);
}


template <typename SampleType>
EnginePtr<SampleType> createEngine (const String& typeID) noexcept
{
	try
	{
		const auto func = findCreationFunction<SampleType> (typeID);
		return func();
	}
	catch (std::out_of_range&)
	{
		return nullptr;
	}
}

template EnginePtr<float>  createEngine (const String&) noexcept;
template EnginePtr<double> createEngine (const String&) noexcept;


template <typename SampleType>
EngineSet<SampleType> getEngineSet (const EngineList& list)
{
	EngineSet<SampleType> engines;

	for (const auto& typeID : list)
		if (auto engine = createEngine<SampleType> (typeID))
			engines.add (std::move (engine));

	return engines;
}

template EngineSet<float>  getEngineSet (const EngineList&);
template EngineSet<double> getEngineSet (const EngineList&);

}  // namespace lemons::dsp::factory
