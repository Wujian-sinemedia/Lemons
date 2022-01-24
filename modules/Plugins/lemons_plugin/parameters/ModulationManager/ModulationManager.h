#pragma once


namespace lemons::plugin
{
/** @ingroup parameter_modulation
	A modulation manager for a plugin.
	This class holds a collection of LFOs that can be mapped to any number of Parameter objects.
 */
class ModulationManager final
{
public:

	using LfoNamingFunc = std::function<String (int)>;

	explicit ModulationManager (const ParameterList& parameterList,
		int											 initialNumLFOs = 0,
		LfoNamingFunc&&								 namingFunc		= nullptr);


	/** @name mod_manager_access_lfos Accessing LFOs */
	///@{

	/** Returns the LFO at the given index in the modulation manager's vector of LFOs.
		The returned pointer may be null if an LFO does not exist at the requested index. \n \n
		This is the least preferable way to retrieve LFO objects; prefer to use getLFOofType(), getLFOatFrequency(), or getLFOwithConnection().
	*/
	[[nodiscard]] LFO* getLFO (int index);

	/** Returns the first LFO found with the given oscillator type.
		The returned pointer may be null if no LFOs have the requested type.
	*/
	[[nodiscard]] LFO* getLFOofType (dsp::osc::OscType type);

	/** Returns the first LFO found with the given frequency.
		The returned pointer may be null if no LFOs have the requested frequency.
	*/
	[[nodiscard]] LFO* getLFOatFrequency (float freq);

	/** Returns the first LFO found that is connected to the specified parameter.
		The returned pointer may be null if no LFOs have the requested connection.
	*/
	[[nodiscard]] LFO* getLFOwithConnection (Parameter& param);

	///@}


	/** @name mod_manager_add_lfos Adding LFOs */
	///@{

	LFO& addLFO();

	LFO& addLFO (std::unique_ptr<LFO> lfo);

	///@}


	void prepareToPlay (int numSamples, double samplerate);

	void finishBlock (int numSamples);

private:

	const ParameterList& paramList;

	ConstructedArray<LFO> lfos;
};

}  // namespace lemons::plugin
