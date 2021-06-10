namespace bav
{

UndoManager::UndoManager (SerializableData& stateToManage)
: state (stateToManage)
{
    
}

bool UndoManager::undo()
{
    if (hasUndo())
    {
        loadState (storedStates[--currentStep]);
        return true;
    }
    
    return false;
}

bool UndoManager::redo()
{
    if (hasRedo())
    {
        loadState (storedStates[++currentStep]);
        return true;
    }
    
    return false;
}

void UndoManager::undoToLastTransaction()
{
    loadState (storedStates[currentStep]);
}

bool UndoManager::hasUndo() const
{
    return (currentStep - 1 >= 0);
}

bool UndoManager::hasRedo() const
{
    return (currentStep + 1) < storedStates.size();
}

void UndoManager::beginNewTransaction (const String& name)
{
    transactionName = name;
}

void UndoManager::endTransaction()
{
    storedStates.emplace_back ({ state.serialize(), transactionName });
}

juce::StringArray UndoManager::getUndoTransactionNames() const
{
    juce::StringArray names;
    
    for (int i = currentStep - 1; i >= 0; --i)
        names.add (storedStates[i].name);
    
    return names;
}

juce::StringArray UndoManager::getRedoTransactionNames() const
{
    juce::StringArray names;
    
    for (int i = currentStep + 1; i < storedStates.size(); ++i)
        names.add (storedStates[i].name);
    
    return names;
}

void UndoManager::loadState (State& stateToLoad)
{
    state.deserialize (stateToLoad.state);
    transactionName = stateToLoad.transactionName;
}

UndoManager::State::State (juce::ValueTree tree, const String& name)
{
    state = tree;
    transactionName = name;
}

}
