/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#pragma once

namespace lemons::dsp
{

/** A kind of engine that contains a chain of other Engine objects.
    Each node in the chain must inherit from AudioEngine, and each node has an individual bypass setting.
    The latency of the entire chain will be the latency of the node with the greatest individual latency.
    @see Engine, BuiltEngineChain
 */
template <typename SampleType>
class EngineChain : public LatencyEngine<SampleType>
{
public:

	virtual ~EngineChain() = default;

	/** Represents a node in an EngineChain. */
	struct Node final
	{
		/** Creates a Node holding the given Engine object. */
		explicit Node (std::unique_ptr<Engine<SampleType>> engineToUse);

		/** Returns the managed Engine object. */
		[[nodiscard]] Engine<SampleType>& getEngine();

		/** Returns a pointer to the managed Engine object. */
		[[nodiscard]] Engine<SampleType>* operator->();

		/** Returns whether this node's individual bypass setting is active or not. */
		[[nodiscard]] bool isBypassed() const noexcept;

		/** Sets this node's individual bypass setting. */
		void setBypassed (bool shouldBeBypassed) noexcept;

		/** Returns this node's current index in its parent EngineChain. Indices start at 0.
		    Be careful storing these; a node's index may change if the EngineChain is manipulated!
		 */
		[[nodiscard]] int getIndex() const noexcept;

	private:
		friend class EngineChain;

		std::unique_ptr<Engine<SampleType>> engine;
		int                                 index { 0 };
		bool                                bypassed { false };
	};

	/** Returns the node at the specified index. */
	[[nodiscard]] Node* getNode (int index);

	/** Returns the node at the specified index. */
	[[nodiscard]] const Node* getNode (int index) const;

	/** Returns a pointer to the first node found that holds an Engine of the specified type.
	    @tparam EngineType The type of Engine object to search for. This type must inherit from Engine<SampleType>.
	    @see getEngineOfType()
	 */
	template <typename EngineType, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	[[nodiscard]] Node* getNodeOfType()
	{
		for (auto* node : nodes)
			if (const auto* typed = dynamic_cast<EngineType*> (node->engine.get()))
				return node;

		return nullptr;
	}

	/** Similar to getNodeOfType(), but returns a pointer to the actual engine object as its derived type, instead of a pointer to its containing node.
	    @tparam EngineType The type of Engine object to search for. This type must inherit from Engine<SampleType>.
	    @see getNodeOfType()
	 */
	template <typename EngineType, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	[[nodiscard]] EngineType* getEngineOfType()
	{
		for (const auto* node : nodes)
			if (auto* engine = dynamic_cast<EngineType*> (node->engine.get()))
				return engine;

		return nullptr;
	}

	/** Adds a node to the end of the chain.
	    @return The index at which the node was added
	 */
	int addNode (Node* newNode);

	/** Creates a node holding an engine of the specified type, and forwards all arguments to the engine's constructor. Adds the node to the end of the chain.
	    @tparam EngineType The type of Engine object that the new node will hold. This type must inherit from Engine<SampleType>.
	 */
	template <typename EngineType, typename... Args, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	Node* createAndAddNode (Args&&... args)
	{
		auto* node = new Node (std::make_unique<EngineType> (std::forward<Args> (args)...));

		node->index = getNextNodeIndex();

		return nodes.add (node);
	}

	/** Adds a node to the front of the chain, at index 0.
	    Note that this may change the indices of any other nodes already in the chain!
	 */
	void addNodeToFront (Node* newNode);

	/** Creates a node holding an engine of the specified type, and forwards all arguments to the engine's constructor. Adds the node to the front of the chain, at index 0.
	    Note that this may change the indices of any other nodes already in the chain!
	    @tparam EngineType The type of Engine object that the new node will hold. This type must inherit from Engine<SampleType>.
	 */
	template <typename EngineType, typename... Args, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	Node* createAndAddNodeToFront (Args&&... args)
	{
		auto* node = new Node (std::make_unique<EngineType> (std::forward<Args> (args)...));

		node->index = 0;

		for (auto* n : nodes)
			n->index += 1;

		return nodes.add (node);
	}

	/** Adds a node after the specified other node.
	    Note that this may change the indices of any other nodes already in the chain!
	    @return The index at which newNode was added
	 */
	int addNodeAfter (Node* newNode, const Node* nodeToInsertAfter);

	/** Creates a node holding an engine of the specified type, and forwards all arguments to the engine's constructor. Adds the node to the chain after the specified other node.
	    Note that this may change the indices of any other nodes already in the chain!
	    @tparam EngineType The type of Engine object that the new node will hold. This type must inherit from Engine<SampleType>.
	 */
	template <typename EngineType, typename... Args, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	Node* createAndAddNodeAfter (const Node* nodeToInsertAfter, Args&&... args)
	{
		if (! nodes.contains (nodeToInsertAfter))
		{
			jassertfalse;
			return nullptr;
		}

		auto* newNode = new Node (std::make_unique<EngineType> (std::forward<Args> (args)...));

		newNode->index = nodeToInsertAfter->index + 1;

		for (int i = newNode->index; i < getNextNodeIndex(); ++i)
			if (auto* node = getNode (i))
				node->index += 1;

		return nodes.add (newNode);
	}

	/** Adds a node before the specified other node.
	    Note that this may change the indices of any other nodes already in the chain!
	    @return The index at which newNode was added
	 */
	int addNodeBefore (Node* newNode, const Node* nodeToInsertBefore);

	/** Creates a node holding an engine of the specified type, and forwards all arguments to the engine's constructor. Adds the node to the chain before the specified other node.
	    Note that this may change the indices of any other nodes already in the chain!
	    @tparam EngineType The type of Engine object that the new node will hold. This type must inherit from Engine<SampleType>.
	 */
	template <typename EngineType, typename... Args, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	Node* createAndAddNodeBefore (const Node* nodeToInsertBefore, Args&&... args)
	{
		if (! nodes.contains (nodeToInsertBefore))
		{
			jassertfalse;
			return nullptr;
		}

		auto* newNode = new Node (std::make_unique<EngineType> (std::forward<Args> (args)...));

		newNode->index = std::max (0, nodeToInsertBefore->index - 1);

		for (int i = newNode->index; i < getNextNodeIndex(); ++i)
			if (auto* node = getNode (i))
				node->index += 1;

		return nodes.add (newNode);
	}

	/** Swaps the indices of two nodes in the chain.
	    @return True if the operation was successful
	 */
	bool swapNodes (Node* node1, Node* node2);

	/** Swaps the indices of the two nodes in the chain at the given indices.
	    @return True if the operation was successful
	 */
	bool swapNodesAtIndices (int index1, int index2);

	/** Removes and deletes a node object from the chain.
	    @return True if the operation succeeds. This will really only fail if you pass a pointer to a Node object that is not in this chain.
	 */
	bool removeNode (Node* node);

	/** Removes and deletes a node object at the specified index from the chain.
	    @return True if the operation succeeds.
	 */
	bool removeNode (int index);

private:
	void renderChunk (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) final;

	void prepared (int blocksize, double samplerate, int numChannels) final;

	void latencyChanged (int newLatency) final;

	void onRelease() final;

	void onReset() final;

	[[nodiscard]] int getNextNodeIndex() const;

	juce::OwnedArray<Node> nodes;
};


/** An EngineChain that you can pass templated Engine types to and it will instantiate them all and add them as nodes to the chain for you.
    For example:
    @code
    template<typename SampleType>
    struct MyFirstEngine : pubic Engine<SampleType>
    {
    };

    template<typename SampleType>
    struct MySecondEngine : pubic Engine<SampleType>
    {
    };

    using MyChain = BuiltEngineChain<float, MyFirstEngine, MySecondEngine>;
    @endcode
    @tparam SampleType The floating-point type that will be used for all the nodes.
    @tparam EngineTypes A list of unspecialized templated types. Each one of these types must inherit from Engine<SampleType>.
    @see Engine, EngineChain
 */
template <typename SampleType, template <typename T> typename... EngineTypes>
class BuiltEngineChain final : public EngineChain<SampleType>
{
public:
	BuiltEngineChain()
	{
		(this->template createAndAddNode<EngineTypes<SampleType>>(), ...);
	}
};

}  // namespace lemons::dsp
