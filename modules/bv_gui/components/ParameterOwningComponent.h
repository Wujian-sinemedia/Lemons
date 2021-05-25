
#pragma once

namespace bav::gui
{
//class ComponentWithParameterConnection : public juce::Component
//{
//public:
//    ComponentWithParameterConnection (Parameter&         parameter,
//                                      juce::ValueTree&   vt,
//                                      juce::UndoManager& um)
//        : state (vt), parameterName (parameter.parameterNameVerbose), undoManager (um)
//    {
//        setName (parameter.parameterNameVerbose);
//        setComponentID (parameter.parameterNameVerbose);
//
//        parameterValue.referTo (state, DefaultValueTreeIds::ParameterValue, nullptr);
//        parameterIsChanging.referTo (
//            state, DefaultValueTreeIds::ParameterIsChanging, nullptr);
//    }
//
//    void setValueAsCompleteGesture (float newNormalizedValue)
//    {
//        if (parameterValue.get() != newNormalizedValue)
//        {
//            beginGesture();
//            parameterValue.setValue (newNormalizedValue, &undoManager);
//            endGesture();
//        }
//    }
//
//    void beginGesture()
//    {
//        if (! parameterIsChanging.get())
//        {
//            undoManager.beginNewTransaction();
//            undoManager.setCurrentTransactionName (TRANS ("Changed") + " "
//                                                   + parameterName);
//            parameterIsChanging.setValue (true, &undoManager);
//        }
//    }
//
//    void endGesture()
//    {
//        if (parameterIsChanging.get())
//            parameterIsChanging.setValue (false, &undoManager);
//    }
//
//    void setValueAsPartOfGesture (float newNormalizedValue)
//    {
//        if (parameterValue.get() != newNormalizedValue)
//            parameterValue.setValue (newNormalizedValue, &undoManager);
//    }
//
//protected:
//    juce::ValueTree state;
//
//    const juce::String parameterName;
//
//    juce::CachedValue< float > parameterValue;
//    juce::CachedValue< bool >  parameterIsChanging;
//
//    juce::UndoManager& undoManager;
//};


}  // namespace bav::gui
