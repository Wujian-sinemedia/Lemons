
namespace lemons::plugin
{

[[nodiscard]] static inline ModulationManager::LfoNamingFunc createLfoNamingFunc (ModulationManager::LfoNamingFunc&& orig)
{
	if (orig != nullptr)
		return orig;

	return [] (int idx) -> String
	{
		return TRANS ("LFO") + " " + String (idx);
	};
}

ModulationManager::ModulationManager (const ParameterList& parameterList,
                                      int                  initialNumLFOs,
                                      LfoNamingFunc&&      namingFunc)
    : paramList (parameterList)
    , lfos (initialNumLFOs, [&, func = createLfoNamingFunc (std::move (namingFunc))]()
            { return new LFO (paramList, func (lfos->size())); })
{
}

LFO* ModulationManager::getLFO (int index)
{
	if (index >= lfos->size()) return nullptr;
	return lfos[index];
}

LFO* ModulationManager::getLFOofType (dsp::osc::OscType type)
{
	for (auto* lfo : lfos)
		if (lfo->getOscillator().getOscType() == type)
			return lfo;

	return nullptr;
}

LFO* ModulationManager::getLFOatFrequency (float freq)
{
	for (auto* lfo : lfos)
		if (lfo->getOscillator().getFrequency() == freq)
			return lfo;

	return nullptr;
}

LFO* ModulationManager::getLFOwithConnection (Parameter& param)
{
	for (auto* lfo : lfos)
		if (lfo->hasConnection (param))
			return lfo;

	return nullptr;
}

LFO& ModulationManager::addLFO()
{
	return lfos.append();
}

LFO& ModulationManager::addLFO (std::unique_ptr<LFO> lfo)
{
	return *lfos->add (std::move (lfo));
}

void ModulationManager::prepareToPlay (int numSamples, double samplerate)
{
	for (auto* lfo : lfos)
		lfo->prepareToPlay (numSamples, samplerate);
}

void ModulationManager::finishBlock (int numSamples)
{
	for (auto* lfo : lfos)
		lfo->finishBlock (numSamples);
}

}  // namespace lemons::plugin
