#pragma once

#include <bv_oscillators/bv_oscillators.h>

namespace bav::plugin
{
class ModulationManager : public SerializableData
{
public:
    using LFO = dsp::LFO;

    void addConnection (Parameter& parameter, LFO& lfo);

    void removeConnection (Parameter& parameter, LFO& lfo);

private:
    void serialize (TreeReflector& ref) final;

    struct Connection : SerializableData
    {
        Connection (Parameter& paramToUse, LFO& lfoToUse);
        ~Connection();

    private:
        void serialize (TreeReflector& ref) final;

        Parameter& parameter;
        LFO&       lfo;
    };
};

}  // namespace bav::plugin
